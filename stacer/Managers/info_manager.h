#ifndef INFO_MANAGER_H
#define INFO_MANAGER_H

#include <QObject>

#include <Info/cpu_info.h>
#include <Info/disk_info.h>
#include <Info/memory_info.h>
#include <Info/network_info.h>
#include <Info/system_info.h>
#include <Info/process_info.h>

#include <Utils/format_util.h>

class InfoManager
{
public:
    static InfoManager *ins();

    quint8 getCpuCoreCount();
    QList<int> getCpuPercents();

    quint64 getSwapUsed();
    quint64 getSwapTotal();
    quint64 getMemUsed();
    quint64 getMemTotal();
    void updateMemoryInfo();

    quint64 getRXbytes();
    quint64 getTXbytes();
    QList<Disk> getDisks();
    void updateDiskInfo();

    QFileInfoList getCrashReports();
    QFileInfoList getAppLogs();
    QFileInfoList getAppCaches();

    void updateProcesses();
    QList<Process> getProcesses();
    QString getUserName();
private:
    InfoManager();
    static InfoManager *_instance;

private:
    CpuInfo ci;
    DiskInfo di;
    MemoryInfo mi;
    NetworkInfo ni;
    SystemInfo si;
    ProcessInfo pi;
};

#endif // INFO_MANAGER_H
