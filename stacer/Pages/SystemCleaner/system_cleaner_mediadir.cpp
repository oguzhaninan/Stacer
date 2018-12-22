#include "system_cleaner_mediadir.h"

void MediaDirData::set_data(QString& key, QStringList& value)
{
    this->directory(&key);
    this->filters(&value);
}

SystemCleanerMediaDir::SystemCleanerMediaDir(QObject *parent) : QObject(parent),
    mContainsAnything(false)
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

bool SystemCleanerMediaDir::containsAnything() const
{
    return mContainsAnything;
}

void SystemCleanerMediaDir::init()
{
    // DOES NOTHING
    addDefaultDirs(false);
}
void SystemCleanerMediaDir::addDirectory(const QString& dir)
{
    mDirectories.insert(dir, QStringList());
	
	auto *a = &mDirectories.constFind(dir).key();
    if(!mContainsAnything)
        mContainsAnything = true;
	emit addedMediaDirectory(const_cast<QString*>(a), nullptr);
}

void SystemCleanerMediaDir::addFilterToDirectory(const QString& filter)
{
    QString emitkey;
    
    if (mDirectories.size() < 2)
    {
        const QString& okey = mDirectories.begin().key();
        QStringList filters = mDirectories.take(okey);
        filters << filter;
        mDirectories.insert(okey, filters);
        emitkey = QString(okey);
    }
    else
    {
        const QString& okey = mDirectories.end().key();
        QStringList filters = mDirectories.take(okey);
        filters << filter;
        mDirectories.insert(okey, filters);
        emitkey = QString(okey);
    }
    
    auto *a = &mDirectories.constFind(emitkey).key();
    auto *b = &mDirectories.constFind(emitkey).value();
    if(!mContainsAnything)
        mContainsAnything = true;
    emit addedMediaDirectory(const_cast<QString*>(a), const_cast<QStringList*>(b));
}
void SystemCleanerMediaDir::addFilterToDirectory(const QString& dir, const QString& filter)
{
    QStringList filters = mDirectories.take(dir);
    filters << filter;
    mDirectories.insert(dir,filters);
    
    auto *a = &mDirectories.constFind(dir).key();
    auto *b = &mDirectories.constFind(dir).value();
    if(!mContainsAnything)
        mContainsAnything = true;
    emit addedMediaDirectory(const_cast<QString*>(a), const_cast<QStringList*>(b));
}

void SystemCleanerMediaDir::addDefaultDirs(const bool do_it)
{
    if (!do_it)
        return;
    
    const QString& default_mediadir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    addDirectory(default_mediadir);
}
