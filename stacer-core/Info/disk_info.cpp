#include "disk_info.h"
#include <QDebug>

DiskInfo::DiskInfo()
{ }

QList<Disk*> DiskInfo::getDisks() const
{
    return disks;
}

void DiskInfo::updateDiskInfo()
{
    disks.clear();
    try {
        QStringList result = CommandUtil::exec("df", { "-Pl" }).split(QChar('\n'));

        QRegExp sep("\\s+");
        for (const QString &line : result.filter(QRegExp("^/")))
        {
            Disk *disk = new Disk;
            QStringList slist = line.split(sep);
            disk->name = slist.takeFirst();
            disk->size = slist.takeFirst().toLong() << 10;
            disk->used = slist.takeFirst().toLong() << 10;
            disk->free = slist.takeFirst().toLong() << 10;
    
            disks << disk;
        }
    } catch (QString &ex) {
        qCritical() << ex;
    }
}
