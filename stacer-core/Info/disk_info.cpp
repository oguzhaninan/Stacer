#include "disk_info.h"
#include <QDebug>

DiskInfo::DiskInfo()
{

}

QList<Disk> DiskInfo::getDisks() const
{
    return disks;
}

void DiskInfo::updateDiskInfo()
{
    try {
        QStringList result = CommandUtil::exec("df -Pl").split(QChar('\n'));

        QRegExp sep("\\s+");
        for (const QString &line : result.filter(QRegExp("^/")))
        {
            Disk disk;
            QStringList slist = line.split(sep);
            disk.size = slist.at(1).toLong() << 10;
            disk.used = slist.at(2).toLong() << 10;
            disk.free = slist.at(3).toLong() << 10;
    
            disks << disk;
        }
    } catch (QString &ex) {
        qCritical() << ex;
    }
}
