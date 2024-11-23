#ifndef NETWORKDISCOVER_H
#define NETWORKDISCOVER_H

#include <QObject>

#include <QQmlEngine>

class NetworkControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList availableWiFiNets READ availableWiFiNets NOTIFY availableWiFiNetsChanged)

public:
    NetworkControl(QObject* parent = nullptr);

    QStringList availableWiFiNets() const;
    Q_INVOKABLE void updateWiFiInfo();
    Q_INVOKABLE bool tryConnect(int idx, const QString& pass);
    Q_INVOKABLE QString getWifiIP();

    static void registerNetworkControl();

signals:
    void availableWiFiNetsChanged();

private:
    void findWiFiIP();

    QStringList m_availableWiFiNets;
    QStringList ip_addrs;
};

#endif // NETWORKDISCOVER_H
