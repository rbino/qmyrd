#pragma once

#include <QObject>

#include <jack/types.h>

class JackClient : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(float volume READ volume NOTIFY volumeChanged)

public:
    JackClient(QObject *parent = nullptr);
    ~JackClient();

    QString name() const;
    float volume() const;

    void setName(const QString &name);

    int static staticProcess(jack_nframes_t nFrames, void *arg);
    void static staticPortConnect(jack_port_id_t a, jack_port_id_t b, int connected, void *arg);

signals:
    void nameChanged();
    void volumeChanged();

private slots:
    void initJack(const QString &name);
    int process(jack_nframes_t nFrames);
    void portConnect(jack_port_id_t a, jack_port_id_t b, bool connected);

private:
    jack_client_t *m_client;
    jack_port_t *m_inputPort;

    QString m_name;
    float m_volume;

    float m_acc;
    int m_accSamples;
};
