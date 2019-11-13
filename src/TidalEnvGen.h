#include "EnvelopeGenerator.h"

#include <QVariant>

class TidalEnvGen : public EnvelopeGenerator
{
    Q_OBJECT

    Q_PROPERTY(QString addressPattern READ addressPattern WRITE setAddressPattern NOTIFY addressPatternChanged)
    Q_PROPERTY(QString matchKey READ matchKey WRITE setMatchKey NOTIFY matchKeyChanged)
    Q_PROPERTY(QVariant matchValue READ matchValue WRITE setMatchValue NOTIFY matchValueChanged)

public:
    TidalEnvGen(QObject *parent = nullptr);
    ~TidalEnvGen();

    QString addressPattern() const;
    QString matchKey() const;
    QVariant matchValue() const;

    void setAddressPattern(const QString &addressPattern);
    void setMatchKey(const QString &matchKey);
    void setMatchValue(QVariant matchValue);

signals:
    void addressPatternChanged();
    void matchKeyChanged();
    void matchValueChanged();

private slots:
    void onMessage(const QString &path, QVariantList args);

private:
    QString m_addressPattern;
    QString m_matchKey;
    QVariant m_matchValue;
};
