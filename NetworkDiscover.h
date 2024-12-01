
#ifndef NETWORKDISCOVER_H
#define NETWORKDISCOVER_H

#include <QObject>

#include <QQmlEngine>


/**
 * @brief The NetworkControl class
 * @details Используя QProcess и API nmcli осуществляет управление состоянием wifi устройства,
 * обновляет список доступных сетей, подключается к указанной сети, вкл/выкл
 */
class NetworkControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList availableWiFiNets READ availableWiFiNets NOTIFY availableWiFiNetsChanged)
    Q_PROPERTY(QString currentIp READ currentIp NOTIFY currentIpChanged FINAL)
    Q_PROPERTY(bool wifiState READ wifiState WRITE setWifiState NOTIFY wifiStateChanged FINAL)
    Q_PROPERTY(int activeSsidIdx READ activeSsidIdx NOTIFY activeSsidIdxChanged FINAL)

public:
    /**
     * @brief NetworkControl
     * @param parent
     */
    NetworkControl(QObject* parent = nullptr);

    /**
     * @brief выполянет регистрацию класса в qml
     */
    static void registerNetworkControl();

    /**
     * @brief возвращает список доступных сетей
     * @return
     */
    QStringList availableWiFiNets() const;

    /**
     * @brief запускает обновление списка доступных сетей
     */
    Q_INVOKABLE void updateWiFiInfo();
    /**
     * @brief попытка соединения с сетью
     * @param idx - индекс подключаемой сети в списке сетей
     * @param pass - пароль сети
     * @return результат подключения
     */
    Q_INVOKABLE bool tryConnect(int idx, const QString& pass);
    /**
     * @brief Возвращает состояние wifi
     * @return
     */
    bool wifiState() const;
    /**
     * @brief установка состояни wifi
     * @details при запуске wifi происходить обновление списка доступных сетей и IP
     * @param true - запустить wifi
     * @param false - отключить wifi
     */
    void setWifiState(bool newWifiState);
    /**
     * @brief Возвращает текущий IP в виде втроки
     * @return
     */
    QString currentIp() const;

    /**
     * @brief индекс текущей активной сети в списке сетей
     * @return индекс
     */
    int activeSsidIdx() const;


signals:
    /**
     * @brief список доступных сетей изменился
     */
    void availableWiFiNetsChanged();
    /**
     * @brief изменилось состояние wifi
     */
    void wifiStateChanged();
    /**
     * @brief изменился текущий IP
     */
    void currentIpChanged();

protected:
    /**
     * @brief Обработка событий внутреннего таймер QObject
     * @details каждое срабатываение таймера проверяется состояние wifi и текущий IP
     * если wifi запущен - обновляется список доступных сетей
     *
     */
    void timerEvent(QTimerEvent *) override;

private:
    /**
     * @brief запрашивает текущее состояние wifi
     * @details устанавливает m_wifiState равным текущем состоянию wifi
     */
    void checkWifiState();
    /**
     * @brief переключение состояния wifi (ВКЛ/ВЫКЛ)
     * @param enable - новое состояние
     */
    void setWifiEnabledState(bool enable);
    /**
     * @brief проверка IPv4 на интерфейсе wlan0
     * @details записывает строку ip-адреса в m_currentIp
     */
    void checkIpAddrOnWlan0();
    /**
     * @brief отправка команды в nmcli
     * @details запускает nmcl комманду череез QProcess с указанным списком аргументов
     * @param command передаваемый список аргументов
     * @param msecTimeout максимальное время ожидания завершения команды
     * @return
     */
    QString nmcliCommand(const QStringList& command, int msecTimeout);

    QStringList m_availableWiFiNets; ///<
    bool m_wifiState; ///<
    QString m_currentIp = ""; ///<
    int m_activeSsidIdx; ///<
};

#endif // NETWORKDISCOVER_H
