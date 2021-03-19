#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include "Utils/file_util.h"
#include "Utils/format_util.h"
#include "Utils/command_util.h"
#include "Info/cpu_info.h"
#include "Info/gpu_info.h"

#define LSCPU_COMMAND "LANG=nl_NL.UTF-8 lscpu"

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT SystemInfo
{
public:
    SystemInfo();

    QString getHostname() const;
    QString getUsername() const;
    QString getPlatform() const;
    QString getDistribution() const;
    QString getKernel() const;

    // CPU information
    QString getCpuModel() const;
    QString getCpuSpeed() const;
    QString getCpuCore() const;


    // GPU information
    QString getGPUCoreString() const;
    int getGPUCore() const;
    QString getGPUDriverVersion() const;
    QString getGPUAcceleratorVersion() const;
    std::vector<double> getGPUTotalMemory() const;
    QStringList getGPUName() const;
    bool sucessGPUInfo;

    

    QFileInfoList getCrashReports() const;
    QFileInfoList getAppLogs() const;
    QFileInfoList getAppCaches() const;

    QStringList getUserList() const;
    QStringList getGroupList() const;

private slots:
private:
    QString cpuCore;
    QString cpuModel;
    QString cpuSpeed;
    QString username;
    QString gpuNodeString;
    QStringList gpuNames;
    int gpuNode;
    QString GPUDriverVersion;
    QString GPUAcceleratorVersion;
    std::vector<double> GPUTotalMemory;
};

#endif // SYSTEMINFO_H
