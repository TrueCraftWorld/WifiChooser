#ifndef WIFILISTMODEL_H
#define WIFILISTMODEL_H

#include <QAbstractListModel>
#include <QObject>

struct WiFiItem {
    QString ssid;
    int presentCounter;
};


class WiFiListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WiFiListModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    QMap<QString, WiFiItem> m_items;

    // QAbstractItemModel interface
public:
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);
};

#endif // WIFILISTMODEL_H
