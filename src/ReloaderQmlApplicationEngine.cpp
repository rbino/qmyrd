#include "ReloaderQmlApplicationEngine.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>
#include <QTimer>

ReloaderQmlApplicationEngine::ReloaderQmlApplicationEngine(QObject *parent)
    : QQmlApplicationEngine(parent)
    , m_watcher(new QFileSystemWatcher(this))
{
    connect(m_watcher, &QFileSystemWatcher::fileChanged, this, &ReloaderQmlApplicationEngine::notifyAndClearCache);
}

bool ReloaderQmlApplicationEngine::setPath(const QString &path)
{
    if (!QFile::exists(path)) {
        qWarning() << "File" << path << "does not exist";
        return false;
    }

    m_watcher->addPath(path);

    return true;
}

void ReloaderQmlApplicationEngine::notifyAndClearCache(const QString &path)
{
    // Some editors (i.e. vim) delete and replace the file
    // Give them some time
    QTimer::singleShot(200, this, [this, path] {
        this->clearComponentCache();
        // This is needed since it's a new file for QFileSystemWatcher
        m_watcher->addPath(path);
        emit componentChanged();
    });
}
