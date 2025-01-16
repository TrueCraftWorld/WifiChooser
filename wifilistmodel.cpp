#include "wifilistmodel.h"

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
        return data.ssid;
    case SsidRole:
        return data.ssid;
    case BssidRole:
        return data.bssid;
    case PresentCounterRole:
        return data.presentCounter;
    default:
        return QVariant();
    }
}

bool WiFiListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

}

void WiFiListModel::addWiFiItem(const WiFiItem &str)
{
    if (m_id.contains(str.ssid))
        return;
    beginInsertRows(QModelIndex(), 0, 1);

    m_ssid.append(str);
    m_id[str.ssid] = 1;
    endInsertRows();
}

void WiFiListModel::removeWiFiItem(int index)
{
    if (index < 0 || index >= dataList.size()) {
        return;
    }

    beginRemoveRows(QModelIndex(), index, index);
    m_id.remove(m_ssid.at(index).ssid);
    m_ssid.removeAt(index);
    endRemoveRows();
}

QHash<int, QByteArray> WiFiListModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[SsidRole] = "ssid";
    roles[BssidRole] = "bssid";
    roles[PresentCounterRole] = "presentCounter";

    return roles;
}


