#pragma once

#include <QObject>

class JackConnectionsCache : public QObject
{
    Q_OBJECT

public:
    static JackConnectionsCache *instance();

private:
    JackConnectionsCache(QObject *parent = nullptr);
    ~JackConnectionsCache();

    static JackConnectionsCache* s_instance;
};
