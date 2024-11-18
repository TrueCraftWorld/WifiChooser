#include "NetworkDiscover.h"

#include <QProcess>
#include <QQmlContext>
#include <QDebug>

NetworkControl::NetworkControl(QObject* parent)
    : QObject(parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    updateWiFiInfo();
    m_availableWiFiNets.clear();
    qDebug() << "i'm alive";
}

QStringList NetworkControl::availableWiFiNets() const
{

    return m_availableWiFiNets;
}

void NetworkControl::updateWiFiInfo()
{
    qDebug() << "press";
    m_availableWiFiNets.clear();
    QProcess process;

    /*nmcli --field SSID device wifi
     * Пользуемся тем фактом, что у нас в системе сеть управлется через NetworkMandger
     * эта команда выводит построчно дочтупные SSID сетей wifi если на таргет системе wifi запущен
     * */
    // запускаем желаемую команду
    process.start("nmcli", QStringList() << "--field" << "SSID" << "device" << "wifi" << "list" << "--rescan" << "yes");

    //ожидаем выполнения (5 сек максимум)
    process.waitForFinished(5000);

    /*считываем весь консольный вывод команды, что-то типа
     * SSID - обязательный заголовок поля
     * NET1 - имена сетей
     * NET2
     * ...
     * NETn
     * */
    QByteArray rawConsoleOutput = process.readAllStandardOutput();
    QString consoleString(rawConsoleOutput);
    QStringList separatedNames = consoleString.split('\n');
    foreach (const QString& name, separatedNames) {
        if (name.isEmpty())
            continue;
        const QString& nameTrim =name.trimmed();
        if (nameTrim != "SSID")
            m_availableWiFiNets.append(nameTrim);

        qDebug() << nameTrim;
    }
    qDebug() << "end update";
    emit availableWiFiNetsChanged();
}

void NetworkControl::registerNetworkControl()
{
    qmlRegisterType<NetworkControl>("BackEnd", 1, 0, "NetworkSearch");
}


