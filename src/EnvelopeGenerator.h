#pragma once

#include <QObject>

class QTimer;

class EnvelopeGenerator : public QObject {
    Q_OBJECT

    Q_PROPERTY(double attack READ attack WRITE setAttack NOTIFY attackChanged)
    Q_PROPERTY(double release READ release WRITE setRelease NOTIFY releaseChanged)
    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged)
    Q_PROPERTY(double value READ value NOTIFY valueChanged)

public:
    EnvelopeGenerator(QObject *parent = nullptr);
    ~EnvelopeGenerator();

    enum Phase { OffPhase, AttackPhase, ReleasePhase };
    Q_ENUM(Phase)

    double attack() const;
    double release() const;
    bool isRunning() const;
    double value() const;

    void setAttack(double attack);
    void setRelease(double release);

    Q_INVOKABLE void trigger();

signals:
    void attackChanged();
    void releaseChanged();
    void runningChanged();
    void valueChanged();

private slots:
    void tick();

private:
    QTimer *m_timer;
    Phase m_currentPhase;
    double m_attack;
    double m_release;
    double m_value;
};
