#include "EnvelopeGenerator.h"

#include <QTimer>

#define TICK_MS 33 // ~30 fps, +/- ~2 ms since it's a CoarseTimer
#define TICK_S ((double) TICK_MS / 1000.0)
#define PEAK_VALUE 1.0
#define DEFAULT_ATTACK 0.0
#define DEFAULT_RELASE 0.2

EnvelopeGenerator::EnvelopeGenerator(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
    , m_currentPhase(OffPhase)
    , m_attack(DEFAULT_ATTACK)
    , m_release(DEFAULT_RELASE)
    , m_value(0)
{
    m_timer->setInterval(33);
    connect(m_timer, &QTimer::timeout, this, &EnvelopeGenerator::tick);
}

EnvelopeGenerator::~EnvelopeGenerator()
{
}

void EnvelopeGenerator::trigger()
{
    m_currentPhase = AttackPhase;
    m_timer->start();
}

void EnvelopeGenerator::tick()
{
    switch (m_currentPhase) {
    case AttackPhase:
        // Handle attack 0
        if (m_attack <= 0) {
            m_value = PEAK_VALUE;
        } else {
            m_value += (TICK_S / m_attack) * PEAK_VALUE;
        }

        if (m_value >= PEAK_VALUE) {
            m_value = PEAK_VALUE;
            m_currentPhase = ReleasePhase;
        }
        emit valueChanged();
        break;
    case ReleasePhase:
        // Handle release 0
        if (m_release <= 0) {
            m_value = 0;
        } else {
            m_value -= (TICK_S / m_release) * PEAK_VALUE;
        }

        if (m_value <= 0) {
            m_value = 0;
            m_currentPhase = OffPhase;
            m_timer->stop();
        }
        emit valueChanged();
        break;
    default:
        break;
    }
}

double EnvelopeGenerator::attack() const
{
    return m_attack;
}

double EnvelopeGenerator::release() const
{
    return m_release;
}

bool EnvelopeGenerator::isRunning() const
{
    return m_currentPhase != OffPhase;
}

double EnvelopeGenerator::value() const
{
    return m_value;
}

void EnvelopeGenerator::setAttack(double attack)
{
    if (m_attack != attack) {
        m_attack = attack;
        emit attackChanged();
    }
}

void EnvelopeGenerator::setRelease(double release)
{
    if (m_release != release) {
        m_release = release;
        emit releaseChanged();
    }
}
