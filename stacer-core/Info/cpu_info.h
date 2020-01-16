#ifndef CPUINFO_H
#define CPUINFO_H

#include <QDebug>
#include <QVector>

#include "Utils/file_util.h"

#define PROC_CPUINFO "/proc/cpuinfo"
#define LSCPU_COMMAND "LANG=nl_NL.UTF-8 lscpu"
#define PROC_LOADAVG "/proc/loadavg"
#define PROC_STAT    "/proc/stat"

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT CpuInfo
{
public:
    quint8 getCpuCoreCount() const;
    QList<int> getCpuPercents() const;
    QList<double> getLoadAvgs() const;
    double getAvgClock() const;
    QList<double> getClocks() const;

private:
    int getCpuPercent(const QList<double> &cpuTimes, const int &processor = 0) const;
};

#endif // CPUINFO_H
