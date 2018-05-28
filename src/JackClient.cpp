#include "JackClient.h"

#include <jack/jack.h>

#include <QDebug>
#include <QtMath>

// 44100 / 60
#define KR_SAMPLES 735

JackClient::JackClient(QObject *parent)
    : QObject(parent)
    , m_client(nullptr)
    , m_volume(0)
    , m_acc(0)
    , m_accSamples(0)
{
    initJack();
}

JackClient::~JackClient()
{
    if (m_client) {
        jack_client_close(m_client);
    }
}

void JackClient::initJack()
{
    const char *clientName = "qmyrd";
    const char *serverName = NULL;
    jack_options_t options = JackNullOption;
    jack_status_t status;

    m_client = jack_client_open(clientName, options, &status, serverName);
    if (!m_client) {
        qWarning() << "jack_client_open() failed, status" << status;
        return;
    }

    if (status & JackServerStarted) {
        qDebug() << "JACK server started";
    }

    if (status & JackNameNotUnique) {
        clientName = jack_get_client_name(m_client);
        qDebug() << "unique name" << clientName << "assigned";
    }

    jack_set_process_callback(m_client, JackClient::staticProcess, (void*)this);

    m_inputPort = jack_port_register (m_client, "input",
                                      JACK_DEFAULT_AUDIO_TYPE,
                                      JackPortIsInput, 0);

    if (!m_inputPort) {
        qWarning() << "Can't open input port";
        return;
    }

    if (jack_activate(m_client)) {
        qWarning() << "Can't activate client";
        return;
    }
}

float JackClient::volume() const
{
    return m_volume;
}

int JackClient::process(jack_nframes_t nFrames)
{
    jack_default_audio_sample_t *in;

    in = reinterpret_cast<jack_default_audio_sample_t*>(jack_port_get_buffer(m_inputPort, nFrames));

    for (unsigned int i=0; i < nFrames; i++) {
        m_acc += qPow(in[i], 2);
        if (++m_accSamples >= KR_SAMPLES) {
            m_volume = qSqrt(m_acc / m_accSamples);
            emit volumeChanged();
            m_acc = 0;
            m_accSamples = 0;
        }
    }

    return 0;
}

int JackClient::staticProcess(jack_nframes_t nFrames, void *arg)
{
    JackClient *client = reinterpret_cast<JackClient*>(arg);
    return client->process(nFrames);
}
