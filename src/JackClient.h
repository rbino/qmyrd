#pragma once

#include <QObject>

class JackClient : public QObject
{
    Q_OBJECT

public:
    JackClient(QObject *parent = nullptr);
    ~JackClient();
};
