#include "wifilistmodel.h"

#include <QDebug>
#include <QTimer>
#include <QRandomGenerator>

const QStringList debugNets{"otherWifi", "SomeWifi","KeepOut","MyButt_5G","HEEELP",
    "I_AM_DUMB", "MARCH","TESLA_","LOOP","POOP",
    "BOBER", "Koldun","hatenaming","KURWA","SomeWifi",
    "SCUM", "WIFI","Harold","shit","theList",};

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
    default:
        return QVariant();
    }
    return QVariant();
}

void WiFiListModel::addWiFiItem(const QString &str)
{
    if (m_id.contains(str))
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

QHash<int, QByteArray> WiFiListModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[SsidRole] = "ssid";
    roles[BssidRole] = "bssid";
    roles[PresentCounterRole] = "presentCounter";

    return roles;
}


