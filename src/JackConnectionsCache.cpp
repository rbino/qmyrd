#include "JackConnectionsCache.h"

JackConnectionsCache *JackConnectionsCache::s_instance;

JackConnectionsCache::JackConnectionsCache(QObject *parent)
    : QObject(parent)
{
}

JackConnectionsCache::~JackConnectionsCache()
{
}

JackConnectionsCache *JackConnectionsCache::instance()
{
    if (!s_instance) {
        s_instance = new JackConnectionsCache();
    }
    return s_instance;
}

void JackConnectionsCache::addConnection(const QString &clientName, const QByteArray &connection)
{
    m_clientToPorts.insert(clientName, connection);
}

void JackConnectionsCache::removeConnection(const QString &clientName, const QByteArray &connection)
{
    m_clientToPorts.remove(clientName, connection);
}

QList<QByteArray> JackConnectionsCache::connections(const QString &clientName)
{
    return m_clientToPorts.values(clientName);
}
