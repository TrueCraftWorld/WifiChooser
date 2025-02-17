#ifndef WIFILISTMODEL_H
#define WIFILISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
// #define DEBUG

struct WiFiItem {
    QString ssid;
    QString bssid; //do we need it since it is non unique?
    int presentCounter;
    bool isConnected = false;
};


class WiFiListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum CustomRows {
        SsidRole = Qt::UserRole + 1,
        BssidRole,
        PresentCounterRole,
        ConnectedRole
    };

    explicit WiFiListModel(QObject *parent = nullptr);

public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    void addWiFiItem(const QString &str);
    void removeWiFiItem(int index);
    void updateWiFiList(const QStringList& currVisibleNets);
    void setActiveSsid(const QString& ssid);

private:
    QMap<QString, int> m_id;
    QList<WiFiItem> m_ssid;
    QString m_activeSsid = "";
};

#endif // WIFILISTMODEL_H
