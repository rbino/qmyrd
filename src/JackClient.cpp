#include "JackClient.h"

#include "JackConnectionsCache.h"

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
    , m_name(QStringLiteral("default"))
{
    initJack(m_name);
}

JackClient::~JackClient()
{
    if (!m_client) {
        return;
    }

    jack_client_close(m_client);
}

void JackClient::initJack(const QString &name)
{
    QByteArray clientName = QStringLiteral("%1_qmyrd").arg(name).toLatin1();
    const char *serverName = NULL;
    jack_options_t options = JackNullOption;
    jack_status_t status;

    m_client = jack_client_open(clientName.constData(), options, &status, serverName);
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
    jack_set_port_connect_callback(m_client, JackClient::staticPortConnect, reinterpret_cast<void *>(this));

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

    QList<QByteArray> cachedConnections = JackConnectionsCache::instance()->connections(m_name);
    for (const QByteArray &connection : cachedConnections) {
        int ret = jack_connect(m_client, connection.constData(), jack_port_name(m_inputPort));
        if (ret && ret != EEXIST) {
            qWarning() << "Couldn't connect to port " << connection << ": error " << ret;
        }
    }
}

QString JackClient::name() const
{
    return m_name;
}

float JackClient::volume() const
{
    return m_volume;
}

void JackClient::setName(const QString &name)
{
    if (name.isEmpty()) {
        qWarning() << "Can't set empty name";
        return;
    }

    if (m_client) {
        jack_client_close(m_client);
    }

    m_name = name;
    emit nameChanged();
    initJack(m_name);
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

void JackClient::portConnect(jack_port_id_t a, jack_port_id_t b, bool connected)
{
    jack_port_t *aPort = jack_port_by_id(m_client, a);
    jack_port_t *bPort = jack_port_by_id(m_client, b);

    QByteArray connection;
    if (jack_port_is_mine(m_client, aPort)) {
        connection = jack_port_name(bPort);
    } else if (jack_port_is_mine(m_client, bPort)) {
        connection = jack_port_name(aPort);
    } else {
        return;
    }

    if (connected) {
        JackConnectionsCache::instance()->addConnection(m_name, connection);
    } else {
        JackConnectionsCache::instance()->removeConnection(m_name, connection);
    }
}

int JackClient::staticProcess(jack_nframes_t nFrames, void *arg)
{
    JackClient *client = reinterpret_cast<JackClient*>(arg);
    return client->process(nFrames);
}

void JackClient::staticPortConnect(jack_port_id_t a, jack_port_id_t b, int connected, void *arg)
{
    JackClient *client = reinterpret_cast<JackClient *>(arg);
    client->portConnect(a, b, connected);
}
