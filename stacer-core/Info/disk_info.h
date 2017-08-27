#ifndef DISKINFO_H
#define DISKINFO_H

#include <QObject>
#include "Utils/command_util.h"
#include "Utils/file_util.h"

#define PROC_MOUNTS "/proc/mounts"

class Disk;

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT DiskInfo : public QObject
{
    Q_OBJECT

public:
    DiskInfo();

    QList<Disk> getDisks() const;
    void updateDiskInfo();

private:
    QList<Disk> disks;
};

class Disk {
public:
    Disk() {}

    quint64 size;
    quint64 free;
    quint64 used;
};

#endif // DISKINFO_H
