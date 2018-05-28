#pragma once

#include <QQmlApplicationEngine>

class QFileSystemWatcher;

class ReloaderQmlApplicationEngine : public QQmlApplicationEngine
{
    Q_OBJECT
public:
    explicit ReloaderQmlApplicationEngine(QObject *parent = nullptr);

    bool setPath(const QString &path);

signals:
    void componentChanged();

private slots:
    void notifyAndClearCache(const QString &path);

private:
    QFileSystemWatcher *m_watcher;
};

