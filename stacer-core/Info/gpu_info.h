#ifndef GPUINFO_H
#define GPUINFO_H

#include "pugixml.hpp"


#include <functional>
#include <QDebug>
#include <QVector>
#include <QString>
#include <string>

#include "Utils/file_util.h"

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT GpuInfo
{
public:

    GpuInfo();

    int load_correct_version();

    std::function<QString(GpuInfo)> getGPUDriverVersion;
    std::function<QString(GpuInfo)> getGPUAcceleratorVersion;
    std::function<int(GpuInfo)> getGPUPhysicalCoreCount;
    std::function<QList<int>(GpuInfo)> getGPUPercents;
    std::function<std::vector<double>(GpuInfo)> getGPUMemoryUsage;
    std::function<std::vector<double>(GpuInfo)> getGPUMemoryTotal;
    std::function<std::vector<double>(GpuInfo)> getGPUUsage;
    std::function<QStringList(GpuInfo)> getGPUName;
    bool finding_GPU_info = false;
    // QList<double> getLoadAvgs() const;
    // double getAvgClock() const;
    // QList<double> getClocks() const;

private:
    pugi::xml_document getXMLnvidiasmi() const;
    struct getGPUDriverVersion_nvidiasmi;
    struct getGPUCudaVersion_nvidiasmi;
    struct getGPUPhysicalCoreCount_nvidiasmi;
    struct getGPUMemoryUsage_nvidiasmi;
    struct getGPUMemoryTotal_nvidiasmi;
    struct getGPUUsage_nvidiasmi;
    struct getGPUName_nvidiasmi;
    //int getCpuPercent(const QList<double> &cpuTimes, const int &processor = 0) const;
};

#endif // CPUINFO_H