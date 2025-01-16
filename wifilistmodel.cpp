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

}

bool WiFiListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

}

void WiFiListModel::addWiFiItem(const QString &str)
{
    beginInsertRows(QModelIndex(), 0, 1);
    m_ssid.prepend(str);
    m_presentCounts.prepend(1);
    endInsertRows();
}

void WiFiListModel::removeWiFiItem(int index)
{
    if (index < 0 || index >= dataList.size()) {
        return;
    }

    beginRemoveRows(QModelIndex(), index, index);
    m_ssid.removeAt(index);
    m_presentCounts.removeAt(index);
    endRemoveRows();
}

QHash<int, QByteArray> WiFiListModel::roleNames() const
{

}


