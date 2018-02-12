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

    quint8 getCpuCoreCount() const;
    QList<int> getCpuPercents() const;

    quint64 getSwapUsed() const;
    quint64 getSwapTotal() const;
    quint64 getMemUsed() const;
    quint64 getMemTotal() const;
    void updateMemoryInfo();

    quint64 getRXbytes() const;
    quint64 getTXbytes() const;
    QList<Disk *> getDisks() const;
    void updateDiskInfo();

    QFileInfoList getCrashReports() const;
    QFileInfoList getAppLogs() const;
    QFileInfoList getAppCaches() const;

    void updateProcesses();
    QList<Process> getProcesses() const;
    QString getUserName() const;
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
