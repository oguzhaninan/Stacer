#include "system_cleaner_mediadir.h"

SystemCleanerMediaDir::SystemCleanerMediaDir(QObject *parent) : QObject(parent)
{
    init();
}

QFileInfoList SystemCleanerMediaDir::fetchFIL(void)
{
    QFileInfoList qfili;

    if (mDirectories.count() < 1)
        return qfili;

    for (auto dir = mDirectories.keyBegin(); dir != mDirectories.keyEnd(); ++dir)
    {

        const QString& key = dir.base().key();
        const QStringList val = mDirectories.value(key, QStringList());
        QDir qdir(key);
        if (!val.isEmpty())
            qdir.setNameFilters(val);
        qfili << qdir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    }

    return qfili;
}

pMediaDirectories SystemCleanerMediaDir::mediaDirectories() const
{
    return &mDirectories;
}

void SystemCleanerMediaDir::init()
{
    // the home directory is our default media directory
    const QString& default_mediadir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    addDirectory(default_mediadir);
}
void SystemCleanerMediaDir::addDirectory(const QString& dir)
{
    mDirectories.insert(dir, QStringList());
}

void SystemCleanerMediaDir::addFilterToDirectory(const QString& filter)
{
    if (mDirectories.size() < 2)
    {
        const QString& okey = mDirectories.begin().key();
        QStringList filters = mDirectories.take(okey);
        filters << filter;
        mDirectories.insert(okey, filters);
    }
    else
    {
        const QString& okey = mDirectories.end().key();
        QStringList filters = mDirectories.take(okey);
        filters << filter;
        mDirectories.insert(okey, filters);
    }
}
void SystemCleanerMediaDir::addFilterToDirectory(const QString& dir, const QString& filter)
{
    QStringList filters = mDirectories.take(dir);
    filters << filter;
    mDirectories.insert(dir,filters);
}
