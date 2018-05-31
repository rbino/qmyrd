#pragma once

#include <QMultiHash>
#include <QObject>

class JackConnectionsCache : public QObject
{
    Q_OBJECT

public:
    static JackConnectionsCache *instance();

    void addConnection(const QString &clientName, const QByteArray &connection);
    void removeConnection(const QString &clientName, const QByteArray &connection);
    QList<QByteArray> connections(const QString &clientName);

private:
    JackConnectionsCache(QObject *parent = nullptr);
    ~JackConnectionsCache();

    static JackConnectionsCache* s_instance;

    QMultiHash<QString, QByteArray> m_clientToPorts;
};
