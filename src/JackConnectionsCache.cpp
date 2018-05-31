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
