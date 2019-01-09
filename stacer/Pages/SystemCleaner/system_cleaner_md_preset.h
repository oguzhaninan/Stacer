#ifndef SYSTEM_CLEANER_MD_PRESET_H
#define SYSTEM_CLEANER_MD_PRESET_H

#include <QException>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSharedDataPointer>
#include <QVariant>
#include "Pages/SystemCleaner/system_cleaner_mediadir.h"

class SystemCleanerMDPreset : public QJsonDocument, public QSharedData
{
protected:
    explicit SystemCleanerMDPreset(const QByteArray& jsondata);

public:
    SystemCleanerMDPreset(const SystemCleanerMDPreset& other);

    virtual ~SystemCleanerMDPreset();

    static SystemCleanerMDPreset* loadPreset(const QString& infile);

    MediaDirData** getData();

    void setMediaDirs(SystemCleanerMediaDir **media_dirs);
    SystemCleanerMediaDir* mediaDirs() const;

private:
    QSharedDataPointer<SystemCleanerMDPreset>       pSelf;
    SystemCleanerMediaDir                          *pMediaDirs;

};

#endif // SYSTEM_CLEANER_MD_PRESET_H
