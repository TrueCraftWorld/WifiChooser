#ifndef NETWORKDISCOVER_H
#define NETWORKDISCOVER_H

#include <QObject>

#include <QQmlEngine>

/**
 * @brief The NetworkControl class
 *
 * @todo возможно стоит проводить проверку списка каждые Х секунды и обновлять только при несовпадении списков
 *      нужно переписать парсер списка для надёжного определения текущего подключения
 *      нужен таймер проверки статуса wifi и адреса
 *      сделать отдельный метод - отправка команды в nmcli
 */


class NetworkControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList availableWiFiNets READ availableWiFiNets NOTIFY availableWiFiNetsChanged)
    Q_PROPERTY(QString currentIp READ currentIp NOTIFY currentIpChanged FINAL)
    Q_PROPERTY(bool wifiState READ wifiState WRITE setWifiState NOTIFY wifiStateChanged FINAL)

public:
    NetworkControl(QObject* parent = nullptr);

    static void registerNetworkControl();

    QStringList availableWiFiNets() const;

    Q_INVOKABLE void updateWiFiInfo();
    Q_INVOKABLE bool tryConnect(int idx, const QString& pass);

    bool wifiState() const;
    void setWifiState(bool newWifiState);

    QString currentIp() const;

    QString nmcliResponce() const;

signals:
    void availableWiFiNetsChanged();
    void wifiStateChanged();
    void currentIpChanged();


protected:
    void timerEvent(QTimerEvent *event) override;

private:
    void findWiFiIP();
    void checkWifiState();
    void setWifiEnabledState(bool enable);
    void checkIpAddrOnWlan0();
    QString nmcliCommand(const QStringList& command, int msecTimeout);
    QStringList m_availableWiFiNets;
    bool m_wifiState;
    QString m_currentIp = "";
};

#endif // NETWORKDISCOVER_H
