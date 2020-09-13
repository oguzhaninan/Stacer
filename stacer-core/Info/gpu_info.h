#ifndef GPUINFO_H
#define GPUINFO_H

#include "pugixml.hpp"

#include <QDebug>
#include <QVector>
#include <string>

#include "Utils/file_util.h"

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT GpuInfo
{
public:
    bool load_correct_version() const();
    std::string getGPUDriverVersion() const;
    std::string getGPUCudaVersion() const;
    int getGPUPhysicalCoreCount() const;
    int getGPUCoreCount() const;
    QList<int> getGPUPercents() const;
    QList<double> getLoadAvgs() const;
    double getAvgClock() const;
    QList<double> getClocks() const;

private:
    int getCpuPercent(const QList<double> &cpuTimes, const int &processor = 0) const;
};

#endif // CPUINFO_H