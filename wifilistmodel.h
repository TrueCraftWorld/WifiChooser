#ifndef WIFILISTMODEL_H
#define WIFILISTMODEL_H

#include <QAbstractListModel>
#include <QObject>

struct WiFiItem {
    QString ssid;
    QString bssid; //?
    int presentCounter;
};


class WiFiListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum CustomRows {
        SsidRole = Qt::UserRole + 1,
        BssidRole,
        PresentCounterRole
    };

    explicit WiFiListModel(QObject *parent = nullptr);

public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QHash<int, QByteArray> roleNames() const;

    void addWiFiItem(const QString& str);
    void removeWiFiItem(int index);

private:
    QMap<QString, int> m_id;
    QVector<WiFiItem> m_ssid;

};

#endif // WIFILISTMODEL_H
