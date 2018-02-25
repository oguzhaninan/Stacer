#include "disk_info.h"
#include <QDebug>

QList<Disk*> DiskInfo::getDisks() const
{
    return disks;
}

void DiskInfo::updateDiskInfo()
{
    disks.clear();

    QList<QStorageInfo> storageInfoList = QStorageInfo::mountedVolumes();

    for(const QStorageInfo &info: storageInfoList) {
        Disk *disk = new Disk;
        disk->name = info.displayName();
        disk->device = info.device();
        disk->size = info.bytesTotal();
        disk->used = info.bytesTotal() - info.bytesFree();
        disk->free = info.bytesFree();

        disks << disk;
    }
}
