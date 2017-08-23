#include "info_manager.h"

InfoManager::InfoManager()
{

}

InfoManager *InfoManager::_instance = NULL;

InfoManager *InfoManager::ins()
{
    if(_instance == NULL)
        _instance = new InfoManager;

    return _instance;
}

QString InfoManager::getUserName()
{
    return si.getUsername();
}

/********************
 * CPU INFORMATION
 *******************/
quint8 InfoManager::getCpuCoreCount()
{
    return ci.getCpuCoreCount();
}

QList<int> InfoManager::getCpuPercents()
{
    return ci.getCpuPercents();
}

/********************
 * MEMORY INFORMATION
 *******************/
void InfoManager::updateMemoryInfo()
{
    mi.updateMemoryInfo();
}

quint64 InfoManager::getSwapUsed()
{
    return mi.getSwapUsed();
}

quint64 InfoManager::getSwapTotal()
{
    return mi.getSwapTotal();
}

quint64 InfoManager::getMemUsed()
{
    return mi.getMemUsed();
}

quint64 InfoManager::getMemTotal()
{
    return mi.getMemTotal();
}

/********************
 * DISK INFORMATION
 *******************/
QList<Disk> InfoManager::getDisks()
{
    return di.getDisks();
}

void InfoManager::updateDiskInfo()
{
    return di.updateDiskInfo();
}

/********************
 * NETWORK INFORMATION
 *******************/
quint64 InfoManager::getRXbytes()
{
    return ni.getRXbytes();
}

quint64 InfoManager::getTXbytes()
{
    return ni.getTXbytes();
}

/********************
 * CLEANER INFORMATION
 *******************/
QFileInfoList InfoManager::getCrashReports()
{
    return si.getCrashReports();
}

QFileInfoList InfoManager::getAppLogs()
{
    return si.getAppLogs();
}

QFileInfoList InfoManager::getAppCaches()
{
    return si.getAppCaches();
}

/********************
 * PROCESSES INFORMATION
 *******************/
void InfoManager::updateProcesses()
{
    pi.updateProcesses();
}

QList<Process> InfoManager::getProcesses()
{
    return pi.getProcessList();
}


