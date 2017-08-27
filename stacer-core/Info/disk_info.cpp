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
        QStringList result = CommandUtil::exec("df -Pl").split("\n");

        foreach(QString line, result.filter(QRegExp("^/")))
        {
            Disk disk;
            disk.size = line.split(QRegExp("\\s+")).at(1).toLong() << 10;
            disk.used = line.split(QRegExp("\\s+")).at(2).toLong() << 10;
            disk.free = line.split(QRegExp("\\s+")).at(3).toLong() << 10;
    
            disks << disk;
        }
    } catch (QString &ex) {
        qCritical() << ex;
    }
}
