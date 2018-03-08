#ifndef FORMAT_UTIL_H
#define FORMAT_UTIL_H

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT FormatUtil
{
public:
    static QString formatBytes(const quint64 &bytes);

public:
    static const quint64 KIBI = 1024;
    static const quint64 MEBI = 1048576;
    static const quint64 GIBI = 1073741824;
    static const quint64 TEBI = 1099511627776;
};

#endif // FORMAT_UTIL_H
