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

    static void registerNetworkControl();

signals:
    void availableWiFiNetsChanged();

private:
    QStringList m_availableWiFiNets;
};

#endif // NETWORKDISCOVER_H
