#ifndef CPUINFO_H
#define CPUINFO_H

#include <QObject>
#include <QDebug>
#include <QVector>

#include "Utils/file_util.h"

#define PROC_CPUINFO "/proc/cpuinfo"
#define PROC_STAT    "/proc/stat"

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT CpuInfo : public QObject
{
    Q_OBJECT

public:
    CpuInfo();

    quint8 getCpuCoreCount();
    QList<int> getCpuPercents();

private:
    int getCpuPercent(QList<double>cpuTimes, int processor = 0);
};

#endif // CPUINFO_H
