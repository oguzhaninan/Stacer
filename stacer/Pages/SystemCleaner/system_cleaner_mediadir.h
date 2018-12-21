#ifndef SYSTEM_CLEANER_MEDIADIR_H
#define SYSTEM_CLEANER_MEDIADIR_H

#include <QObject>
#include <QMap>
#include <QStringList>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QStandardPaths>

typedef QMap<QString, QStringList> MediaDirectories;
typedef const MediaDirectories* pMediaDirectories;

class SystemCleanerMediaDir : public QObject
{
    Q_OBJECT

    Q_PROPERTY(pMediaDirectories mediaDirectories READ mediaDirectories)

public:
    explicit SystemCleanerMediaDir(QObject *parent = nullptr);

    QFileInfoList fetchFIL(void);

    pMediaDirectories mediaDirectories() const;
signals:

private slots:
    void init();

public slots:
    void addDirectory(const QString& dir);
    void addFilterToDirectory(const QString& dir);
    void addFilterToDirectory(const QString& dir, const QString& filter);

private:
    MediaDirectories mDirectories;
};

#endif // SYSTEM_CLEANER_MEDIADIR_H
