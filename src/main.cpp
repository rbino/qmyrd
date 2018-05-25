#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "JackClient.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<JackClient>("com.rbino.qmyrd", 1, 0, "JackClient");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/QmyrdCanvas.qml")));

    return app.exec();
}
