#ifndef FORMAT_UTIL_H
#define FORMAT_UTIL_H

#include <QObject>
#include <QDebug>

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT FormatUtil : public QObject
{
    Q_OBJECT

public:
    static QString formatBytes(quint64 bytes);

private:
    FormatUtil();

    static const quint64 KIBI = 1L << 10;
    static const quint64 MEBI = 1L << 20;
    static const quint64 GIBI = 1L << 30;
    static const quint64 TEBI = 1L << 40;
    static const quint64 PEBI = 1L << 50;
    static const quint64 EXBI = 1L << 60;

};

#endif // FORMAT_UTIL_H
