#include "info_manager.h"

InfoManager *InfoManager::instance = nullptr;

InfoManager *InfoManager::ins()
{
    if(! instance){
        instance = new InfoManager;
    }

    return instance;
}

QString InfoManager::getUserName() const
{
    return si.getUsername();
}

QStringList InfoManager::getUserList() const
{
    return si.getUserList();
}

QStringList InfoManager::getGroupList() const
{
    return si.getGroupList();
}

/*
 * CPU Provider
 */
quint8 InfoManager::getCpuCoreCount() const
{
    return ci.getCpuCoreCount();
}

QList<int> InfoManager::getCpuPercents() const
{
    return ci.getCpuPercents();
}

QList<double> InfoManager::getCpuLoadAvgs() const
{
    return ci.getLoadAvgs();
}

double InfoManager::getCpuClock() const
{
    return ci.getAvgClock();
}

/*
 * Memory Provider
 */
void InfoManager::updateMemoryInfo()
{
    mi.updateMemoryInfo();
}

quint64 InfoManager::getSwapUsed() const
{
    return mi.getSwapUsed();
}

quint64 InfoManager::getSwapTotal() const
{
    return mi.getSwapTotal();
}

quint64 InfoManager::getMemUsed() const
{
    return mi.getMemUsed();
}

quint64 InfoManager::getMemTotal() const
{
    return mi.getMemTotal();
}

/*
 * Disk Provider
 */
QList<Disk*> InfoManager::getDisks() const
{
    return di.getDisks();
}

void InfoManager::updateDiskInfo()
{
    di.updateDiskInfo();
}

QList<quint64> InfoManager::getDiskIO()
{
    return di.getDiskIO();
}

QList<QString> InfoManager::getFileSystemTypes()
{
    return di.fileSystemTypes();
}

QList<QString> InfoManager::getDevices()
{
    return di.devices();
}

/********************
 * Network Provider
 *******************/
quint64 InfoManager::getRXbytes() const
{
    return ni.getRXbytes();
}

quint64 InfoManager::getTXbytes() const
{
    return ni.getTXbytes();
}

/********************
 * System Provider
 *******************/
QFileInfoList InfoManager::getCrashReports() const
{
    return si.getCrashReports();
}

QFileInfoList InfoManager::getAppLogs() const
{
    return si.getAppLogs();
}

QFileInfoList InfoManager::getAppCaches() const
{
    return si.getAppCaches();
}

/********************
 * Process Provider
 *******************/
void InfoManager::updateProcesses()
{
    pi.updateProcesses();
}

QList<Process> InfoManager::getProcesses() const
{
    return pi.getProcessList();
}


