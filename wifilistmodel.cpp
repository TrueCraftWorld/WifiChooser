#include "wifilistmodel.h"

#include <QDebug>

WiFiListModel::WiFiListModel(QObject *parent)
    : QAbstractListModel{parent}
{}

int WiFiListModel::rowCount(const QModelIndex &parent) const
{
    return m_ssid.size();
}

QVariant WiFiListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_ssid.size())
        return QVariant();

    const WiFiItem& data = m_ssid.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        qDebug() << data.ssid;
        return data.ssid;
    case SsidRole:
        qDebug() << data.ssid;
        return data.ssid;
    case BssidRole:
        return data.bssid;
    case PresentCounterRole:
        return data.presentCounter;
    default:
        return QVariant();
    }
}

void WiFiListModel::addWiFiItem(const QString &str)
{
    if (m_id.contains(str))
        return;
    beginInsertRows(QModelIndex(), 0, 1);
    WiFiItem item;
    item.ssid = str;
    item.presentCounter =1;

    m_ssid.prepend(item);
    m_id[str] = 0;
    endInsertRows();
}

void WiFiListModel::updateWiFiList(const QStringList &currVisibleNets)
{
    foreach (const auto& item, currVisibleNets) {
        if (m_id.contains(item)) {
            m_ssid[m_id.value(item)].presentCounter++;
        } else {
            addWiFiItem(item);
        }
    }

    QList<WiFiItem>::iterator iter = m_ssid.begin();
    int a = 0;
    while (iter != m_ssid.end()) {
        iter->presentCounter--;
        if (iter->presentCounter <= 0) {
            m_id.remove(iter->ssid);
            beginRemoveRows(QModelIndex(), a, a);
            iter = m_ssid.erase(iter);
            endRemoveRows();
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


