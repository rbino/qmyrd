#include "OscSocket.h"

#include <oscpack/osc/OscPrintReceivedElements.h>

#include <QColor>
#include <QDebug>
#include <QNetworkDatagram>
#include <QUdpSocket>

OscSocket *OscSocket::s_instance;

OscSocket::OscSocket(int port, QObject *parent)
    : QObject(parent)
    , m_socket(new QUdpSocket(this))
{
    m_socket->bind(QHostAddress::LocalHost, port);

    connect(m_socket, &QIODevice::readyRead, this, &OscSocket::receiveData);
}

OscSocket::~OscSocket()
{
}

void OscSocket::init(int port, QObject *parent)
{
    s_instance = new OscSocket(port, parent);
}

OscSocket *OscSocket::instance()
{
    if (Q_UNLIKELY(!s_instance)) {
        qWarning() << "OscSocket is uninitialized, call OscSocket::init first!";
    }
    return s_instance;
}

void OscSocket::receiveData()
{
    while (m_socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_socket->receiveDatagram();
        QByteArray data = datagram.data();
        try {
            osc::ReceivedPacket pkt(data.data(), data.size());
            if (pkt.IsBundle()) {
                // TODO: handle bundles
                continue;
            }
            QVariantList args;
            osc::ReceivedMessage msg(pkt);
            for (auto it = msg.ArgumentsBegin(); it != msg.ArgumentsEnd(); ++it) {
                args << argumentToVariant(*it);
            }
            QString path = QString::fromStdString(msg.AddressPattern());
            emit messageReceived(path, args);
        } catch (osc::Exception& e) {
            qWarning() << "Exception handling OSC message: " << e.what();
        }
    }
}

QVariant OscSocket::argumentToVariant(const osc::ReceivedMessageArgument& arg)
{
    switch( arg.TypeTag() ){
    case osc::TRUE_TYPE_TAG:
        return true;

    case osc::FALSE_TYPE_TAG:
        return false;

    case osc::NIL_TYPE_TAG:
        return QVariant();

    case osc::INT32_TYPE_TAG:
        return arg.AsInt32Unchecked();

    case osc::FLOAT_TYPE_TAG:
        return arg.AsFloatUnchecked();

    case osc::CHAR_TYPE_TAG:
        return arg.AsCharUnchecked();

    case osc::RGBA_COLOR_TYPE_TAG:
        {
            qint32 color = arg.AsRgbaColorUnchecked();
            int r = (color>>24) & 0xFF;
            int g = (color>>16) & 0xFF;
            int b = (color>>8) & 0xFF;
            int a = color & 0xFF;
            return QColor(r, g, b, a);
        }

    case osc::INT64_TYPE_TAG:
        return (qlonglong)arg.AsInt64Unchecked();

    case osc::DOUBLE_TYPE_TAG:
        return arg.AsDoubleUnchecked();

    case osc::STRING_TYPE_TAG:
        return QString::fromStdString(arg.AsStringUnchecked());

    case osc::BLOB_TYPE_TAG:
        {
            const void *data;
            int size;
            arg.AsBlobUnchecked(data, size);
            return QByteArray((char *)data, size);
        }

    // TODO: handle these
    case osc::INFINITUM_TYPE_TAG:
    case osc::MIDI_MESSAGE_TYPE_TAG:
    case osc::TIME_TAG_TYPE_TAG:
    case osc::SYMBOL_TYPE_TAG:
    case osc::ARRAY_BEGIN_TYPE_TAG:
    case osc::ARRAY_END_TYPE_TAG:
    default:
        return QVariant();
    }
}
