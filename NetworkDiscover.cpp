#include "NetworkDiscover.h"

#include <QProcess>
#include <QQmlContext>
#include <QDebug>
#include <QNetworkInterface>
#include <QTimer>

#include <string>


/* читщит nmcli
 * Пользуемся тем фактом, что у нас в системе сеть управлется через NetworkMandger
 * --wait X - заменить стд ожидание в 90 сек на Х
 * --terse - заменить форматирование вывода на скрипточитаемое
 * --mode multiline - выводить каждое поле на новой строке
 *
 * nmcli radio  wifi - статус вайфай
 *
 * nmcli radio wifi on/off - включить\выключить вайфай
 *
 * nmcli -g IP4.ADDRESS device show wlan0 - получить только значение поля IP4.ADDRESS для устройства wlan0
 *
 * nmcli device wifi connect "$SSID" password "$PASSWORD" - подклбчиться к указанной сети с переданным паролем
 *
 * nmcli connection show --active - список активных соединений
 * nmcli --field TYPE,NAME connection show --active оно же с указанием желаемых полей
 *
 *  nmcli --field SSID,IN-USE device wifi - вывод доступных wifi-соединений с указанными полями
 * весь консольный вывод команды,
 * SSID,IN-USE - обязательные заголовоки полей, значения полей разделены : из-за --terse,
 * NET1: - имена сетей
 * NET2:* - подключенная сеть
 * ...
 * NETn:

 * */

NetworkControl::NetworkControl(QObject* parent)
    : QObject(parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    checkWifiState();
    if (m_wifiState) {

        updateWiFiInfo();
    }
    startTimer(10000);
    m_availableWiFiNets.clear();
}

QStringList NetworkControl::availableWiFiNets() const
{
    return m_availableWiFiNets;
}

void NetworkControl::updateWiFiInfo()
{
    m_availableWiFiNets.clear();

    QString consoleString = nmcliCommand(QStringList() << "--terse" << "--field" << "SSID,IN-USE" << "device" << "wifi",
                                         5000);
    m_activeSsidIdx = -1;

    QStringList wifiList = consoleString.split('\n');

    for (int idx = 0; idx < wifiList.size(); ++idx) {
        QStringList check = wifiList.at(idx).split(':');
        if (check.size() > 1 && check.at(1).contains('*')) {
            m_activeSsidIdx = idx;
        }
        m_availableWiFiNets.append(check.at(0));
    }

    emit availableWiFiNetsChanged();
}

bool NetworkControl::tryConnect(int idx, const QString &pass)
{
    if (idx < 0 || idx >= m_availableWiFiNets.size())
        return false;
    m_currentIp = "-----";

    //защита от пробелов в имени
    QString ssid_name = QString("'%1'").arg(m_availableWiFiNets.at(idx));
    QString passWrapped = QString("'%1'").arg(pass);

    QString consoleString = nmcliCommand(QStringList() << "--wait" << "5" << "device" << "wifi" << "connect" << ssid_name.toStdString().c_str() << "password" << passWrapped.toStdString().c_str(),
                                         5500);
    if (consoleString.contains("Ошибка")) {
        m_currentIp = "0";
        return false;
    }
    updateWiFiInfo();
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
    nmcliCommand(QStringList() << "radio" << "wifi" << (enable ? "on" : "off"),
                 500);
}

void NetworkControl::checkIpAddrOnWlan0()
{
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
    if (m_wifiState)
        updateWiFiInfo();
}



int NetworkControl::activeSsidIdx() const
{
    return m_activeSsidIdx;
}

