#ifndef UPDATECLIENT_H
#define UPDATECLIENT_H

#include <QTcpSocket>
#include <QObject>
#include <QDataStream>
#include <QQmlEngine>

#include "package.h"

class UpdateClient : public QObject
{
    Q_OBJECT
public:
    explicit UpdateClient(QObject *parent = nullptr);
    Q_INVOKABLE void requestUpdate();

    /**
     * @brief выполянет регистрацию класса в qml
     */
    static void registerUpdateClient();
private:
    void receiveFile();
    void clearNetworkData();

private:
    TransferData data;

    bool flag;                      //是否可以同步目录列表，true可是同步，false不可以
    int SYNFlag;                //请求文件同步标志位
    int DOWNFlag;               //下载文件标志位
    QString tempFileName;

    int bytesAwaited= -1;
    void readSocket();
    QTcpSocket* updateSocket;
    QByteArray updateFile;
    QByteArray message;
    QDataStream in;
    QString m_FileName;
    bool headerReaded;
};

#endif // UPDATECLIENT_H
