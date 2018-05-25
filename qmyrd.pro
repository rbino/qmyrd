TEMPLATE = app
TARGET = qmyrd

QT += quick
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS QT_NO_CAST_FROM_ASCII

HEADERS += src/JackClient.h
SOURCES += src/JackClient.cpp src/main.cpp
RESOURCES += resources.qrc
