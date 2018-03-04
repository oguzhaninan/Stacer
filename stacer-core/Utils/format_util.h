#ifndef FORMAT_UTIL_H
#define FORMAT_UTIL_H

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT FormatUtil
{
public:
    static QString formatBytes(const quint64 &bytes);

private:
    static const quint64 KIBI = 1UL << 10UL;
    static const quint64 MEBI = 1UL << 20UL;
    static const quint64 GIBI = 1UL << 30UL;
    static const quint64 TEBI = 1UL << 40UL;

};

#endif // FORMAT_UTIL_H
