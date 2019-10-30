#pragma once

#include <QObject>

#include <oscpack/osc/OscReceivedElements.h>

#include <QVariant>

class QUdpSocket;

class OscSocket : public QObject
{
    Q_OBJECT

public:
    ~OscSocket();

    static void init(int port, QObject *parent = nullptr);
    static OscSocket *instance();

signals:
    void messageReceived(const QString &path, const QVariantList &args);

private slots:
    void receiveData();

private:
    OscSocket(int port, QObject *parent = nullptr);
    QVariant argumentToVariant(const osc::ReceivedMessageArgument &arg);

    static OscSocket *s_instance;

    QUdpSocket *m_socket;
};
