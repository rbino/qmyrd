#pragma once

#include <QObject>

#include <jack/types.h>

class JackClient : public QObject
{
    Q_OBJECT

    Q_PROPERTY(float volume READ volume NOTIFY volumeChanged)

public:
    JackClient(QObject *parent = nullptr);
    ~JackClient();

    float volume() const;

    int static staticProcess(jack_nframes_t nFrames, void *arg);

signals:
    void volumeChanged();

private slots:
    void initJack();
    int process(jack_nframes_t nFrames);

private:
    jack_client_t *m_client;
    jack_port_t *m_inputPort;

    float m_volume;

    float m_acc;
    int m_accSamples;
};
