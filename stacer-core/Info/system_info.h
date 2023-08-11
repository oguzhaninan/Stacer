#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include "Utils/file_util.h"
#include "Utils/format_util.h"
#include "Utils/command_util.h"
#include "Info/cpu_info.h"

#define LSCPU_COMMAND "LANG=nl_NL.UTF-8 lscpu"

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT SystemInfo
{
public:
    SystemInfo();

    static QString getHostname() ;
    static QString getPlatform() ;
    static QString getDistribution() ;
    static QString getKernel() ;
    QString getCpuModel() const;
    QString getCpuSpeed() const;
    QString getCpuCore() const;
    QString getUsername() const;

    static QFileInfoList getCrashReports() ;
    static QFileInfoList getAppLogs() ;
    static QFileInfoList getAppCaches() ;

    static QStringList getUserList() ;
    static QStringList getGroupList() ;

private slots:
private:
    QString cpuCore;
    QString cpuModel;
    QString cpuSpeed;
    QString username;
};

#endif // SYSTEMINFO_H
