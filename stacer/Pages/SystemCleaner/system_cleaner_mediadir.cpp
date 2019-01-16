#include <QDebug>
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
    QStringList fil_grandtotal;

    if (mDirectories.count() < 1)
        return qfili;

    for (const auto& filter : mDirectories.values())
    {
        fil_grandtotal << filter;
    }
    for (const auto& path : mDirectories.keys())
    {
        QDir dir(path);
        dir.setNameFilters(fil_grandtotal);
        qfili += dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    }

    qDebug("QFileInfoList for SystemCleanerMediaDir == %d", qfili.count());

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

MediaDirData SystemCleanerMediaDir::operator [](const int i)
{
    int h = i;
    int j = 0;

    pMediaDirectories pmd = &this->mDirectories;


    if (mDirectories.count() < i)
        throw;

    for (const auto& k : mDirectories.keys())
    {
        if (j == h)
        {
            break;
        }
        j = ++j;
    }

    return [&j,&pmd] { MediaDirData x; x.set_data(pmd->keys()[j], pmd->values()[j]); return x; }();
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
}

void SystemCleanerMediaDir::addFilterToDirectory(const QString& filter)
{
    QString emitkey;
    
    if (mDirectories.size() < 2)
    {
        const QString& okey = mDirectories.begin().key();
        QStringList filters = mDirectories.take(okey);
        if (filter.length() > 0)
        {
            filters << filter;
            mDirectories.insert(okey, filters);
        }
        emitkey = QString(okey);
    }
    else
    {
        const QString& okey = mDirectories.end().key();
        QStringList filters = mDirectories.take(okey);
        if (filter.length() > 0)
        {
            filters << filter;
            mDirectories.insert(okey, filters);
        }
        emitkey = QString(okey);
    }
    
    auto *a = &mDirectories.constFind(emitkey).key();
    auto *b = &mDirectories.constFind(emitkey).value();
    if(!mContainsAnything)
        mContainsAnything = true;
    emit addedMediaDirectory(const_cast<QString*>(a), const_cast<QStringList*>(b), filter);
}
void SystemCleanerMediaDir::addFilterToDirectory(const QString& dir, const QString& filter, const bool emitsig)
{
    addDirectory(dir);
    
    if (filter.length() > 0)
    {
        QStringList filters = mDirectories.take(dir);
        filters << filter;
        mDirectories.insert(dir,filters);
    }
    
    auto *a = &mDirectories.constFind(dir).key();
    auto *b = &mDirectories.constFind(dir).value();
    if(!mContainsAnything)
        mContainsAnything = true;
    /*
     * emitsig only disables the MDD signal below
     */
    if(!emitsig)
        return;
    emit addedMediaDirectory(const_cast<QString*>(a), const_cast<QStringList*>(b), filter);
}

void SystemCleanerMediaDir::addDefaultDirs(const bool do_it)
{
    if (!do_it)
        return;
    
    const QString& default_mediadir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    addDirectory(default_mediadir);
}

void SystemCleanerMediaDir::addMDDs(MediaDirData **mdds, const size_t mdds_len)
{
    QString blank;
    QStringList blankel;
    int control_it = 0;
    
    for (int i = 0; i < mdds_len; i++)
    {
        if ((QString&)mdds[i]->directory() == blank)
            continue;
        
        if (!((QStringList&)mdds[i]->filters() == blankel))
        {
            if (i > 0 && ( (QStringList&)mdds[i]->filters() == (QStringList&)mdds[i-1]->filters() ))
            {
                addFilterToDirectory((QString&)mdds[i]->directory(), blank);
            }
            
            for (const auto& str : (QStringList&)mdds[i]->filters())
            {
                if (control_it == 0)
                {
                    addFilterToDirectory((QString&)mdds[i]->directory(), str);
                }
                else
                {
                    addFilterToDirectory((QString&)mdds[i]->directory(), str, false);
                }
                control_it += 1;
            }
        }
        emit mediaDataRead(mdds[i]);
    }
}

void SystemCleanerMediaDir::addDirByData(MediaDirData& data)
{
    addDirectory(data.directory());

    for (const auto& s : data.filters())
    {
        addFilterToDirectory(data.directory(),s,false);
    }
}
