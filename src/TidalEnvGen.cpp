#include "TidalEnvGen.h"

#include "OscSocket.h"

TidalEnvGen::TidalEnvGen(QObject *parent)
    : EnvelopeGenerator(parent)
{
    connect(OscSocket::instance(), &OscSocket::messageReceived, this, &TidalEnvGen::onMessage);
}

TidalEnvGen::~TidalEnvGen()
{
}

QString TidalEnvGen::addressPattern() const
{
    return m_addressPattern;
}

QString TidalEnvGen::matchKey() const
{
    return m_matchKey;
}

QVariant TidalEnvGen::matchValue() const
{
    return m_matchValue;
}

void TidalEnvGen::setAddressPattern(const QString &addressPattern)
{
    if (m_addressPattern != addressPattern) {
        m_addressPattern = addressPattern;
        emit addressPatternChanged();
    }
}

void TidalEnvGen::setMatchKey(const QString &matchKey)
{
    if (m_matchKey != matchKey) {
        m_matchKey = matchKey;
        emit matchKeyChanged();
    }
}

void TidalEnvGen::setMatchValue(QVariant matchValue)
{
    if (m_matchValue != matchValue) {
        m_matchValue = matchValue;
        emit matchValueChanged();
    }
}

void TidalEnvGen::onMessage(const QString &path, QVariantList args)
{
    // Without this it doesn't make sense to try
    if (m_matchKey.isEmpty()) {
        return;
    }

    if (!m_addressPattern.isEmpty() && m_addressPattern != path) {
        return;
    }

    // Check if args is multiple of 2 since we expect args coming from Tidal (with oShape Nothing)
    // being an array of (key, value) pairs
    if (args.length() % 2 != 0) {
        return;
    }

    for (int i = 0; i < args.length(); i += 2) {
        QString key = args.at(i).toString();
        if (key != m_matchKey) {
            continue;
        }

        QVariant value = args.at(i + 1);
        if (m_matchValue.isNull() || m_matchValue == value) {
            // We have a match, trigger EnvelopeGenerator
            trigger();
        }
    }
}
