#include "updateclient.h"

#include <QFile>
#include <QHostAddress>
#include <QQmlContext>
#include <QDebug>
#include <QFileInfo>
#include "protocolcommand.h"
#include "package.h"
#include "updateConfig.h"

UpdateClient::UpdateClient( QObject *parent)
    : QObject(parent)
{
    qDebug() << "meBorn";

    flag = false;
    SYNFlag = 0;
    DOWNFlag = 0;

    data.payloadSize = 64*1024;
    data.localFile = nullptr;
    data.bytesWritten = 0;
    data.bytesToWrite = 0;

    clearNetworkData();

}

void UpdateClient::requestUpdate()
{
    qDebug() << "request Update";
    updateSocket = new QTcpSocket(this);

    QHostAddress updateHost(UPDATE_SERV_IP);

    connect(updateSocket, &QTcpSocket::connected, this, [this](){
        headerReaded=false;
        qDebug() << "connected to Host";

    });
    // connect(updateSocket, &QTcpSocket::readyRead, this, &UpdateClient::readSocket);
    connect(updateSocket, &QTcpSocket::readyRead, this, &UpdateClient::receiveFile);
    connect(updateSocket, &QTcpSocket::bytesWritten, this, &UpdateClient::receiveFile);
    updateFile.clear();

    updateSocket->bind(updateHost, 11111);
    updateSocket->connectToHost(updateHost,11111);
}

void UpdateClient::registerUpdateClient()
{
    qmlRegisterType<UpdateClient>("BackEnd", 1, 0, "UpdateClient");
}

void UpdateClient::receiveFile()
{

    int downflag = 0  ,  synfilelistflag = 0,   transferfileflag = 0;
    qint32 temp;

    if(updateSocket->bytesAvailable()<=0)
    {
        return;
    }
    QDataStream in(updateSocket);
    in.setVersion(QDataStream::Qt_5_0);
    if(data.bytesReceived <= sizeof(qint64)*3)
    {
        if(updateSocket->bytesAvailable() >= sizeof(qint64)*3
            && (data.fileNameSize==0))
        {
            in >> data.totalBytes >> data.command
                >> data.fileNameSize >> temp;
            data.bytesReceived += sizeof(qint64)*3;
        }
        if(updateSocket->bytesAvailable() >= data.fileNameSize
            && data.fileNameSize!=0)
        {

            in >> data.fileName;
            data.bytesReceived += data.fileNameSize;
        }
    }
    switch(data.command)
    {
    case _TRANSFER_FILE_ :
    {
        transferfileflag = 1;
        if(data.fileNameSize != 0)
        {
            if(SYNFlag == 1) {
                tempFileName = "./FileList/";

            } else if(DOWNFlag == 1) {
                tempFileName = "./DownloadFile/";

            } else {
                tempFileName = "/usr/share/qtpr/";

            }
            tempFileName += data.fileName;
            qDebug()<<tempFileName;
            if (!data.localFile || !data.localFile->isOpen()) {
                data.localFile = new QFile(tempFileName);
                if(!data.localFile->open(QFile::WriteOnly)) {
                    qDebug()<<"open local file error!";
                    return;
                }
            }
        }
    }
    case _TRANSFER_ACK_ :
    {
        qDebug()<<"Send file success!";
    }
    break;
    default:
        qDebug()<<"Receive command nulity!";
    }
    if(data.bytesReceived < data.totalBytes)
    {
        data.bytesReceived += updateSocket->bytesAvailable();
        data.dataBlock = updateSocket->readAll();
        data.localFile->write(data.dataBlock);
        data.dataBlock.resize(0);
    }
    if(data.bytesReceived == data.totalBytes)
    {
        clearNetworkData();
        if(transferfileflag == 1)
        {
            transferfileflag = 0;
            data.localFile->close();
            qDebug()<<"Receive file success!";
        }
        else if(synfilelistflag == 1)
        {
            synfilelistflag = 0;
            qDebug()<<"Request file list success!";
        }
        else if(downflag == 1)
        {
            downflag = 0;
            qDebug()<<"Download file success!";
        }
    }
}

void UpdateClient::clearNetworkData()
{
    data.totalBytes = 0;
    data.bytesReceived = 0;
    data.fileNameSize = 0;
    data.dataBlock.resize(0);
}

void UpdateClient::readSocket()
{
    qDebug() << "read";
    if (bytesAwaited == -1 && updateSocket->bytesAvailable() >=128 ) {

        QByteArray data = updateSocket->readAll();
        qDebug() << "qwerty";
        updateFile.clear();
        QByteArray header;
        header.append(header.mid(0, 128));
        // in >> header;
        // if (!in.commitTransaction())
        //     return;
        qDebug() << header.size() ;
        QString headerStr = header;
        updateFile.append(header.mid(128));

        QString fileName = headerStr.split(",")[0].split(":")[1];
        int dotIdx = fileName.lastIndexOf('.');
        QString fileSuffix = "";
        if (dotIdx != -1){
            fileSuffix = headerStr.mid(dotIdx);
        }
        QString saveFilePath = "/usr/share/qtpr/" + fileName;

        QFileInfo info(saveFilePath);
        while (info.exists()){
            info.setFile(saveFilePath + '.' + fileSuffix);
        }
        m_FileName = info.absoluteFilePath();

        QString fileSize = headerStr.split(",")[1].split(":")[1];
        bytesAwaited = fileSize.toInt();
        qDebug() << m_FileName;
        qDebug() << fileSize.toInt();
qDebug() << updateFile.size() ;
        // qDebug() << "headerRead";
        // qDebug() << count;
        // qDebug() << "fileLeft";
        // qDebug() << bytesAwaited;
        // updateSocket->readAll();

    // } else /*if (updateSocket->bytesAvailable() >= bytesAwaited)*/ {
    } else if (updateSocket->bytesAvailable() > 0) {
        // in.startTransaction();
        QByteArray data = updateSocket->readAll();
        // in >> data;

        if (data.size())
            updateFile.append(data);

        qDebug() << "readwww";
        qDebug() << updateSocket->bytesAvailable();
    }
    qDebug() << updateFile.size() ;
    if (updateFile.size() == bytesAwaited) {
        if (!in.commitTransaction()) {
            // return;
            qDebug() << "nonononono";
        }
        QFile file(m_FileName);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(updateFile);
            file.close();
            qDebug() << "filewrite";
        }
        bytesAwaited = -1;
    }

    // in.startTransaction();

    // if (in.commitTransaction() == false) {
    //     return;
    // }


    // updateFile = header.mid(128);

    // if (file.open(QIODevice::WriteOnly)) {
    //     file.write(updateFile);
    // }

    // updateSocket->disconnect();
    // updateSocket-
}

// void NetworkControl::registerNetworkControl()
// {
//     qmlRegisterType<NetworkControl>("BackEnd", 1, 0, "NetworkSearch");
// }
