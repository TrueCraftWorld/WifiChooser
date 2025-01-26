#include "wifilistmodel.h"

#include <QDebug>
#include <QTimer>
#include <QRandomGenerator>

#ifdef DEBUG
const QStringList debugNets{"otherWifi", "SomeWifi","KeepOut","MyButt_5G","HEEELP",
    "I_AM_DUMB", "MARCH","TESLA_","LOOP","POOP",
    "BOBER", "Koldun","hatenaming","KURWA","SomeWifi",
    "SCUM", "WIFI","Harold","shit","theList"};
#endif

WiFiListModel::WiFiListModel(QObject *parent)
    : QAbstractListModel{parent}
{
#ifdef DEBUG

    QTimer * timer = new QTimer(this);
    timer->setInterval(10000);
    connect(timer, &QTimer::timeout, this, [timer, this]() {
        int newListSize = QRandomGenerator::global()->bounded(debugNets.size()-1);
        newListSize++;
        QVector<quint32> vector;
        vector.resize(newListSize);
        QRandomGenerator::global()->fillRange(vector.data(), vector.size());
        QStringList list;
        for (int i:vector) {
            int k = abs(i);
            list.append(debugNets.at(k%(debugNets.size())));
        }
        updateWiFiList(list);
    });
    timer->start();

#endif
}

int WiFiListModel::rowCount(const QModelIndex &parent) const
{
    return m_ssid.size();
}

QVariant WiFiListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_ssid.size())
        return QVariant();
    int row = index.row();
    const WiFiItem& data = m_ssid.at(row);

    switch (role) {
    case SsidRole:
        return data.ssid;
    case BssidRole:
        return data.bssid;
    case PresentCounterRole:
        return data.presentCounter;
    case ConnectedRole:
        return data.isConnected;
    default:
        return QVariant();
    }
    return QVariant();
}

// bool WiFiListModel::setData(const QModelIndex &index, const QVariant &value, int role)
// {
//     if (!index.isValid() || index.row() >= m_ssid.size() || role != ConnectedRole)
//         return false;

//     int row = index.row();
//     bool result = value.toBool();
//     for (int i = 0; i < m_ssid.size(); ++i) {
//         m_ssid[i].isConnected = false;
//     }
//     m_ssid[row].isConnected = result;
//     emit dataChanged(QModelIndex(), QModelIndex());

//     return true;
// }

void WiFiListModel::addWiFiItem(const QString &str)
{
    if (m_id.contains(str) || str.isEmpty())
        return;
    beginInsertRows(QModelIndex(), 0, 0);
    WiFiItem item;
    item.ssid = str;
    item.presentCounter =1;

    m_ssid.prepend(item);
    m_id[str] = 0;
    endInsertRows();
    // emit dataChanged(QModelIndex(),QModelIndex());
}

void WiFiListModel::updateWiFiList(const QStringList &currVisibleNets)
{
    // qDebug() << currVisibleNets;

    foreach (const auto& item, currVisibleNets) {
        if (m_id.contains(item)) {
            m_ssid[m_id.value(item)].presentCounter = 1;
        } else {
            addWiFiItem(item);
        }
    }

    QList<WiFiItem>::iterator iter = m_ssid.begin();
    int a = 0;
    while (iter != m_ssid.end()) {
        iter->presentCounter--;
        if (iter->presentCounter < 0) {
            beginRemoveRows(QModelIndex(), a, a);
            m_id.remove(iter->ssid);
            iter = m_ssid.erase(iter);
            endRemoveRows();
            emit dataChanged(QModelIndex(),QModelIndex());
        } else {
            m_id[iter->ssid] = a;
            a++;
            iter++;
        }
    }
}

void WiFiListModel::setActiveSsid(const QString &ssid)
{
    if (m_id.contains(m_activeSsid)) {
        m_ssid[m_id[m_activeSsid]].isConnected = false;
        emit dataChanged(index(m_id[m_activeSsid]), index(m_id[m_activeSsid]));
        // emit dataChanged(QModelIndex(), QModelIndex());
        // qDebug()<<m_activeSsid<< "old";

    }
    if (m_id.contains(ssid)) {
        m_ssid[m_id[ssid]].isConnected = true;
        emit dataChanged(index(m_id[ssid]), index(m_id[ssid]));
        // emit dataChanged(QModelIndex(), QModelIndex());
        // qDebug()<<ssid << "new";
    }
    m_activeSsid = ssid;
}

QHash<int, QByteArray> WiFiListModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[SsidRole] = "ssid";
    roles[BssidRole] = "bssid";
    roles[PresentCounterRole] = "presentCounter";
    roles[ConnectedRole] = "isconnected";

    return roles;
}


