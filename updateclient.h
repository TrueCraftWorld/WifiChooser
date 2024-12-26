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

    bool flag;
    int SYNFlag;
    int DOWNFlag;
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
