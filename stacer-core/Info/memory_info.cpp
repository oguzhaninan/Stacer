#include "memory_info.h"

MemoryInfo::MemoryInfo():
    memTotal(0),
    memAvailable(0),
    memUsed(0)
{ }

void MemoryInfo::updateMemoryInfo()
{
    QStringList lines = FileUtil::readListFromFile(PROC_MEMINFO)
            .filter(QRegExp("^MemTotal|^MemAvailable|^SwapTotal|^SwapFree"));

    #define getValue(l) lines.at(l).split(QRegExp("\\s+")).at(1).toLong() << 10;
    memTotal = getValue(0);
    memAvailable = getValue(1);
    swapTotal = getValue(2);
    swapFree = getValue(3)
    #undef getValue

    memUsed = (memTotal - memAvailable);
    swapUsed = (swapTotal - swapFree);
}

quint64 MemoryInfo::getSwapUsed() const
{
    return swapUsed;
}

quint64 MemoryInfo::getSwapFree() const
{
    return swapFree;
}

quint64 MemoryInfo::getSwapTotal() const
{
    return swapTotal;
}

quint64 MemoryInfo::getMemUsed() const
{
    return memUsed;
}

quint64 MemoryInfo::getMemAvailable() const
{
    return memAvailable;
}

quint64 MemoryInfo::getMemTotal() const
{
    return memTotal;
}
