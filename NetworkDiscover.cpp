#include "NetworkDiscover.h"

#include <QProcess>
#include <QQmlContext>
#include <QDebug>
#include <QNetworkInterface>

#include <QtConcurrent>


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

namespace  {
QString nmcliCommand(CommPtr command)
{
    if (command.isNull())
        return QString();
    if (command->msecTimeout < 0)
        command->msecTimeout =100;

    QProcess process;
    process.setEnvironment(QStringList("LC_ALL=C"));
    process.start("nmcli", command->commandLine);
    // qDebug() << "nmcli " << command->commandLine;
    if (process.state() == QProcess::Running){
        process.terminate();
    }
    process.waitForFinished(command->msecTimeout);

    return QString(process.readAllStandardOutput());

}
}

NetworkControl::NetworkControl(QObject* parent)
    : QObject(parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    m_model = new WiFiListModel(this);

    QQmlEngine::setObjectOwnership(m_model, QQmlEngine::CppOwnership);

    connect(&m_watcher, &QFutureWatcher<int>::finished, this, &NetworkControl::slotHandleNmcliResponse);
    connect(&m_timer, &QTimer::timeout, this, &NetworkControl::slotLaunchNextCommand);

    checkWifiState();
    updateWiFiInfo();

    m_timer.start(500); ///ну за глаза учитывая, что nmcli штука не быстрая

    startTimer(10000);
}

QStringList NetworkControl::availableWiFiNets() const
{
    return m_availableWiFiNets;
}

void NetworkControl::updateWiFiInfo()
{
    if (m_searchSuspend)
        return;

    CommPtr ptr (new Command(QStringList() << "--terse" << "--field" << "SSID,IN-USE" << "device" << "wifi" << "list" << "--rescan" << "yes",
                            Command::comCheckVisibleNetworks,
                            5000));
    addCommand(ptr);

}

bool NetworkControl::tryConnect(const QString &ssid, const QString &pass)
{
    QString ssid_name = QString("%1").arg(ssid);
    QString passWrapped = QString("%1").arg(pass);
    CommPtr ptr (new Command(QStringList() << "--wait" << "7" << "dev" << "wifi" << "connect" << ssid_name.toStdString().c_str() << "password" << passWrapped.toStdString().c_str(),
                             Command::comConnectWifi,
                             7000));
    // qDebug() <<  "--wait" << "5" << "device" << "wifi" << "connect" << ssid_name.toStdString().c_str() << "password" << passWrapped.toStdString().c_str();
    addCommand(ptr);

    return true;
}

void NetworkControl::registerNetworkControl()
{
    qmlRegisterType<NetworkControl>("BackEnd", 1, 0, "NetworkSearch");
    qmlRegisterType<WiFiListModel>("BackEnd", 1, 0, "WiFiModel");
}

void NetworkControl::checkWifiState()
{
    CommPtr ptr (new Command(QStringList()  << "radio" << "wifi",
                            Command::comCheckWifi,
                            500));
    addCommand(ptr);
}

void NetworkControl::setWifiEnabledState(bool enable)
{
    if (enable)
        m_searchSuspend = false;

    CommPtr ptr (new Command(QStringList() << "radio" << "wifi" << (enable ? "on" : "off"),
                            Command::comSetWifi,
                            500));
    addCommand(ptr);
}

void NetworkControl::checkIpAddrOnWlan0()
{
    //Исключетельно устройств-зависимый код. Но мы гарантированно знаем название устройства wifi в системе и этим пользуемся
    //ожидаем выполнения (0.5 сек максимум)
    CommPtr ptr (new Command(QStringList() << "-g" << "IP4.ADDRESS" << "device" << "show" << "wlan0",
                            Command::comGetIp,
                            500));
    addCommand(ptr);
}

void NetworkControl::addCommand(CommPtr command, bool isUrgent)
{
    if (isUrgent)
        m_commandQueue.clear();
    m_commandQueue.enqueue(command);
}

void NetworkControl::slotLaunchNextCommand()
{
    if (m_watcher.isRunning() || m_commandQueue.empty() || isBusy/* || !m_currentCommand.isNull()*/)
        return;
    isBusy = true;
    m_currentCommand = m_commandQueue.dequeue();
    QFuture<QString> future = QtConcurrent::run(nmcliCommand, m_currentCommand);
    m_watcher.setFuture(future);
}

bool NetworkControl::wifiState() const
{
    return m_wifiState;
}

void NetworkControl::setWifiState(bool enable)
{
    setWifiEnabledState(enable);
    checkWifiState();
    updateWiFiInfo();
    checkIpAddrOnWlan0();
}

QString NetworkControl::currentIp() const
{
    return m_currentIp;
}

void NetworkControl::timerEvent(QTimerEvent */*event*/)
{
    if (m_searchSuspend)
        return;
    checkWifiState();
    checkIpAddrOnWlan0();
    updateWiFiInfo();
}

void NetworkControl::slotHandleNmcliResponse()
{
    QString response = m_watcher.future();
    qDebug() << response;
    switch (m_currentCommand->type) {
    case Command::comCheckVisibleNetworks:
    {
        m_availableWiFiNets.clear();
        int activeSsidIdx = -1;
        QStringList wifiList = response.split('\n');

        for (int idx = 0; idx < wifiList.size(); ++idx) {
            QStringList check = wifiList.at(idx).split(':');
            if (check.size() > 1 && check.at(1).contains('*')) {
                activeSsidIdx = idx;
            }
            m_availableWiFiNets.append(check.at(0));
        }
#ifndef DEBUG
        m_model->updateWiFiList(m_availableWiFiNets);
        if (activeSsidIdx != -1) {
            m_model->setActiveSsid(m_availableWiFiNets.at(activeSsidIdx));
            qDebug()<<m_availableWiFiNets.at(activeSsidIdx) << "______________";
        }
#endif
        // emit availableWiFiNetsChanged();
    }
        break;
    case Command::comCheckWifi:
    {
        bool newState = m_wifiState;
        if (response.contains("enabled")) {
            newState = true;
            checkIpAddrOnWlan0();
        } else if (response.contains("disabled")) {
            newState = false;
        }

        if (m_wifiState != newState) {
            m_wifiState = newState;
            emit wifiStateChanged();
        }
    }
        break;
    case Command::comConnectWifi:
    {
        m_currentIp = "-----";
        bool res = true;
        if (response.contains("Ошибка")) {
            m_currentIp = "0";
            res = false;
            // break;
        }
        emit commandFinished(res);
        if (res) {
            updateWiFiInfo();
            checkIpAddrOnWlan0();
        }
        // updateWiFiInfo();
        m_searchSuspend = false;
    }
        break;
    case Command::comGetIp:
    {
        if (m_currentIp != response) {
            m_currentIp = response;
            emit currentIpChanged();
        }
    }
        break;
    case Command::comSetWifi:
        ///nothing to check
        break;
    default:
        break;
    }
    isBusy=false;
    m_currentCommand.reset();
}



int NetworkControl::activeSsidIdx() const
{
    return m_activeSsidIdx;
}

void NetworkControl::suspendNetSearch()
{
    m_searchSuspend = true;
}

void NetworkControl::resumeNetSearch()
{
    m_searchSuspend = false;
}

bool Command::empty() const
{
    return isEmpty;
}

WiFiListModel *NetworkControl::wifiModel()
{
    return m_model;
}
