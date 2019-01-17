#ifndef SYSTEM_CLEANER_MEDIADIR_H
#define SYSTEM_CLEANER_MEDIADIR_H

#include <QObject>
#include <QMap>
#include <QStringList>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QStandardPaths>

class MediaDirData {
public:
    MediaDirData() = default;
    MediaDirData(const MediaDirData& other) = default;
    virtual ~MediaDirData() { }

    const QString& directory(QString *dir) {if(dir){mKey=QString(*dir);} return const_cast<QString&>(mKey);}
    QString& directory()  const { return const_cast<QString&>(mKey); }
    const QStringList& filters(QStringList *filt) {if(filt){QStringList qsl;std::copy(filt,filt+1,&qsl);mValues.clear();mValues << qsl;} return const_cast<QStringList&>(mValues);}
    QStringList& filters() const { return const_cast<QStringList&>(mValues); }

    virtual void set_data(QString& key, QStringList& value);
private:
    QString mKey;
    QStringList mValues;
};

typedef QMap<QString, QStringList> MediaDirectories;
typedef const MediaDirectories* pMediaDirectories;

class SystemCleanerMediaDir : public QObject
{
    Q_OBJECT

    Q_PROPERTY(pMediaDirectories mediaDirectories READ mediaDirectories)

    Q_PROPERTY(bool containsAnything MEMBER mContainsAnything READ containsAnything NOTIFY firstDirAdded)

public:
    explicit SystemCleanerMediaDir(QObject *parent = nullptr);

    QFileInfoList fetchFIL(void);

    pMediaDirectories mediaDirectories() const;
    bool containsAnything() const;

    virtual MediaDirData at(const int i);
    virtual MediaDirData operator [](const int i);

signals:
    void   addedMediaDirectory(QString *dir, QStringList *filters, const QString& lastfilter);
	void   mediaDataRead(const MediaDirData *data);
    void   firstDirAdded(void);

private slots:
    void init();

public slots:
    void addDirectory(const QString& dir);
    void addFilterToDirectory(const QString& dir);
    void addFilterToDirectory(const QString& dir, const QString& filter, const bool emitsig=true);
    
    void addMDDs(MediaDirData **mdds, const size_t mdds_len);
    void addDirByData(MediaDirData& data);
    
    void removeDirectory(const QString& dir);

    void reinitialize(void);
private:
    MediaDirectories mDirectories;
    bool             mContainsAnything;
};

#endif // SYSTEM_CLEANER_MEDIADIR_H
