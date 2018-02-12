#ifndef DISKINFO_H
#define DISKINFO_H

#include "Utils/command_util.h"
#include "Utils/file_util.h"

#define PROC_MOUNTS "/proc/mounts"

class Disk;

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT DiskInfo
{
public:
    DiskInfo();

    QList<Disk*> getDisks() const;
    void updateDiskInfo();

private:
    QList<Disk*> disks;
};

class Disk {
public:
    Disk() {}

    QString name;
    quint64 size;
    quint64 free;
    quint64 used;
};

#endif // DISKINFO_H
