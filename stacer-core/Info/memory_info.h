#ifndef MEMORYINFO_H
#define MEMORYINFO_H

#include "Utils/file_util.h"

#define PROC_MEMINFO "/proc/meminfo"

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT MemoryInfo
{
public:
    MemoryInfo();

    void updateMemoryInfo();

    quint64 getMemTotal() const;
    quint64 getMemAvailable() const;
    quint64 getMemUsed() const;

    quint64 getSwapTotal() const;
    quint64 getSwapFree() const;
    quint64 getSwapUsed() const;

private:
    // memory
    quint64 memTotal;
    quint64 memAvailable;
    quint64 memUsed;

    // swap
    quint64 swapTotal;
    quint64 swapFree;
    quint64 swapUsed;
};

#endif // MEMORYINFO_H
