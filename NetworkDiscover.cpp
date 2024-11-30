#include "NetworkDiscover.h"

#include <QProcess>
#include <QQmlContext>
#include <QDebug>
#include <QNetworkInterface>
#include <QTimer>

#include <string>


/* читщит nmcli
 *
 * nmcli radio  wifi - статус вайфай
 *
 * nmcli radio wifi on/off - включить\выключить вайфай
 *
 * nmcli -g IP4.ADDRESS device show wlan0
 *
 * nmcli device wifi connect "$SSID" password "$PASSWORD"
 * nmcli --wait 15 device wifi connect "$SSID" password "$PASSWORD"
 *
 * nmcli connection show --active
 * nmcli --field TYPE,NAME connection show --active
 *
 * nmcli --field SSID device wifi
 * nmcli --field SSID,IN-USE device wifi
 * nmcli --field SSID,IN-USE --mode multiline device wifi
 * */

NetworkControl::NetworkControl(QObject* parent)
    : QObject(parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    checkWifiState();
    if (m_wifiState) {

        updateWiFiInfo();
    }
    startTimer(5000);
    m_availableWiFiNets.clear();
}

QStringList NetworkControl::availableWiFiNets() const
{
    return m_availableWiFiNets;
}

void NetworkControl::updateWiFiInfo()
{
    m_availableWiFiNets.clear();
    /*nmcli --field SSID device wifi
     * Пользуемся тем фактом, что у нас в системе сеть управлется через NetworkMandger
     * */
    // запускаем желаемую команду

    //ожидаем выполнения (5 сек максимум)

    /*считываем весь консольный вывод команды, что-то типа
     * SSID - обязательный заголовок поля
     * NET1 - имена сетей
     * NET2
     * ...
     * NETn
     * */
    QString consoleString = nmcliCommand( QStringList() << "--field" << "SSID" << "device" << "wifi" << "list" << "--rescan" << "yes",
                                                               5000);
    QStringList separatedNames = consoleString.split('\n');
    foreach (const QString& name, separatedNames) {
        if (name.isEmpty())
            continue;
        const QString& nameTrim = name.trimmed();
        if (nameTrim != "SSID")
            m_availableWiFiNets.append(nameTrim);

    }
    emit availableWiFiNetsChanged();
}

bool NetworkControl::tryConnect(int idx, const QString &pass)
{
    //nmcli --wait 5 device wifi connect SSID_or_BSSID password password

    if (idx < 0 || idx >= m_availableWiFiNets.size())
        return false;
    m_currentIp = "-----";
    //защита от пробелов в имени
    QString ssid_name = QString("%1").arg(m_availableWiFiNets.at(idx));
    // list << "device" << "wifi" << "connect" << ssid_name.toStdString().c_str() << "password" << pass.toStdString().c_str();
    QString consoleString = nmcliCommand(QStringList() << "--wait" << "50" << "device" << "wifi" << "connect" << ssid_name.toStdString().c_str() << "password" << pass.toStdString().c_str(),
                                         50500);
    qDebug() << "connect" << ssid_name.toStdString().c_str() << "password" << pass.toStdString().c_str();
    if (consoleString.contains("Ошибка")) {
        m_currentIp = "0";
        return false;
    }
    // checkIpAddrOnWlan0();
    return true;
}

void NetworkControl::registerNetworkControl()
{
    qmlRegisterType<NetworkControl>("BackEnd", 1, 0, "NetworkSearch");
}

void NetworkControl::checkWifiState()
{
    QString consoleString = nmcliCommand(QStringList()  << "radio" << "wifi",
                                 500);
    bool newState = m_wifiState;
    if (consoleString.contains("enabled")) {
        newState = true;
        checkIpAddrOnWlan0();
    } else if (consoleString.contains("disabled")) {
        newState = false;
    }

    if (m_wifiState != newState) {
        m_wifiState = newState;
        emit wifiStateChanged();
    }
}

void NetworkControl::setWifiEnabledState(bool enable)
{
    //ожидаем выполнения (0.5 сек максимум)
    nmcliCommand(QStringList() << "radio" << "wifi" << (enable ? "on" : "off"),
                 500);
}

void NetworkControl::checkIpAddrOnWlan0()
{
    // nmcli -g IP4.ADDRESS device show wlan0
    //Исключетельно устройств-зависимый код. Но мы гарантированно знаем название устройства wifi в системе и этим пользуемся
    //ожидаем выполнения (0.5 сек максимум)
    QString consoleString = nmcliCommand(QStringList() << "-g" << "IP4.ADDRESS" << "device" << "show" << "wlan0",
                                         500);
    if (m_currentIp != consoleString) {
        m_currentIp = consoleString;
        emit currentIpChanged();
    }
}

QString NetworkControl::nmcliCommand(const QStringList &command, int msecTimeout)
{
    if (msecTimeout < 0)
        msecTimeout =100;
    QProcess process;
    process.start("nmcli", command);
    process.waitForFinished(msecTimeout);
    if (process.state() == QProcess::Running){
        process.terminate();
    }
    return QString(process.readAllStandardOutput()).trimmed();
}

bool NetworkControl::wifiState() const
{
    return m_wifiState;
}

void NetworkControl::setWifiState(bool enable)
{
    setWifiEnabledState(enable);
    checkWifiState();
    if (m_wifiState) {
        updateWiFiInfo();
    }
    checkIpAddrOnWlan0();
}

QString NetworkControl::currentIp() const
{
    return m_currentIp;
}

void NetworkControl::timerEvent(QTimerEvent */*event*/)
{
    checkWifiState();
    checkIpAddrOnWlan0();
}


