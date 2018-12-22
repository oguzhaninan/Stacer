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
    virtual ~MediaDirData() { }

    const QString& directory(QString *dir=nullptr) {if(dir){mKey=QString(*dir);} return const_cast<QString&>(mKey);}
    QString& directory() { return mKey; }
    const QStringList& filters(QStringList *filt=nullptr) {if(filt){QStringList qsl;std::copy(filt,filt+1,&qsl);mValues.clear();mValues << qsl;} return const_cast<QStringList&>(mValues);}
    QStringList& filters() { return mValues; }

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

signals:
    void   addedMediaDirectory(QString *dir, QStringList *filters);
    void   firstDirAdded(void);

private slots:
    void init();

public slots:
    void addDirectory(const QString& dir);
    void addFilterToDirectory(const QString& dir);
    void addFilterToDirectory(const QString& dir, const QString& filter);
    
    void addDefaultDirs(const bool do_it=false);

private:
    MediaDirectories mDirectories;
    bool             mContainsAnything;
};

#endif // SYSTEM_CLEANER_MEDIADIR_H
