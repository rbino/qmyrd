TEMPLATE = app
TARGET = qmyrd

QT += quick
CONFIG += c++11 link_pkgconfig
QT_CONFIG -= no-pkg-config
PKGCONFIG += jack

DEFINES += QT_DEPRECATED_WARNINGS QT_NO_CAST_FROM_ASCII

HEADERS += src/JackClient.h \
           src/JackConnectionsCache.h \
           src/ReloaderQmlApplicationEngine.h
SOURCES += src/main.cpp \
           src/JackClient.cpp \
           src/JackConnectionsCache.cpp \
           src/ReloaderQmlApplicationEngine.cpp
RESOURCES += resources.qrc
