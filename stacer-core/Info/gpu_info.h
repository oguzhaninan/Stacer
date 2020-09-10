#ifndef GPUINFO_H
#define GPUINFO_H

#include <QDebug>
#include <QVector>

#include "Utils/file_util.h"

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT GpuInfo
{
public:
    int getCpuPhysicalCoreCount() const;
    int getCpuCoreCount() const;
    QList<int> getCpuPercents() const;
    QList<double> getLoadAvgs() const;
    double getAvgClock() const;
    QList<double> getClocks() const;

private:
    int getCpuPercent(const QList<double> &cpuTimes, const int &processor = 0) const;
};

#endif // CPUINFO_H