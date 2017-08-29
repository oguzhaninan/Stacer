#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include "Utils/file_util.h"
#include "Utils/format_util.h"
#include "Utils/command_util.h"
#include "Info/cpu_info.h"

#define PROC_CPUINFO "/proc/cpuinfo"

#include "stacer-core_global.h"
class STACERCORESHARED_EXPORT SystemInfo
{
public:
    SystemInfo();

    QString getHostname();
    QString getPlatform();
    QString getDistribution();
    QString getKernel();
    QString getCpuModel();
    QString getCpuSpeed();
    QString getCpuCore();

    QFileInfoList getCrashReports();
    QFileInfoList getAppLogs();
    QFileInfoList getAppCaches();

    QString getUsername() const;

private:
    QString cpuCore;
    QString cpuModel;
    QString cpuSpeed;

    QString username;
};

#endif // SYSTEMINFO_H
