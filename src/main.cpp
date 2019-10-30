#include <QCommandLineParser>
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

#include "ReloaderQmlApplicationEngine.h"

#include "EnvelopeGenerator.h"
#include "JackClient.h"
#include "OscSocket.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<EnvelopeGenerator>("com.rbino.qmyrd", 1, 0, "EnvelopeGenerator");
    qmlRegisterType<JackClient>("com.rbino.qmyrd", 1, 0, "JackClient");

    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("qmyrd makes your rectangles dance"));
    parser.addHelpOption();
    parser.addPositionalArgument(QStringLiteral("path"), QStringLiteral("Path to root QML file to be live reloaded"));
    parser.process(app);

    QStringList posArgs = parser.positionalArguments();
    if (posArgs.isEmpty()) {
        parser.showHelp();
    }

    ReloaderQmlApplicationEngine engine;

    QString qmlPath = posArgs.value(0);
    if (qmlPath.isEmpty()) {
        qWarning() << "You must specify the QML with the QMYRD_MAIN_QML env var";
        return 1;
    }

    if (!engine.setPath(qmlPath)) {
        return 1;
    }

    OscSocket::init(7771, &engine);

    QUrl qmlUrl = QUrl::fromLocalFile(qmlPath);
    engine.rootContext()->setContextProperty(QStringLiteral("$ComponentUrl"), qmlUrl);
    engine.rootContext()->setContextProperty(QStringLiteral("$Engine"), &engine);
    engine.load(QUrl(QStringLiteral("qrc:/qml/QmyrdCanvas.qml")));

    return app.exec();
}
