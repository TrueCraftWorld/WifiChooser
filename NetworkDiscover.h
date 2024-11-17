#ifndef NETWORKDISCOVER_H
#define NETWORKDISCOVER_H

#include <QObject>

#include <QQmlEngine>

class NetworkDiscover : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList availableWiFiNets READ availableWiFiNets NOTIFY availableWiFiNetsChanged)

public:
    NetworkDiscover(QObject* parent = nullptr);

    QStringList availableWiFiNets() const;
    Q_INVOKABLE void updateWiFiInfo();

signals:
    void availableWiFiNetsChanged();

private:
    QStringList m_availableWiFiNets;
};

#endif // NETWORKDISCOVER_H
