#include "format_util.h"

#include <QString>

FormatUtil::FormatUtil()
{

}

QString FormatUtil::formatBytes(quint64 bytes)
{
#define formatUnit(v, u, t) QString().sprintf("%.1f %s", \
    ((double) v / (double) u), t)

    if (bytes == 1L) // bytes
        return QString("%1 byte").arg(bytes);
    else if (bytes < KIBI) // bytes
      return QString("%1 bytes").arg(bytes);
    else if (bytes < MEBI) // KiB
      return formatUnit(bytes, KIBI, "KiB");
    else if (bytes < GIBI) // MiB
      return formatUnit(bytes, MEBI, "MiB");
    else if (bytes < TEBI) // GiB
      return formatUnit(bytes, GIBI, "GiB");
    else if (bytes < PEBI) // TiB
      return formatUnit(bytes, TEBI, "TiB");
    else if (bytes < EXBI) // PiB
      return formatUnit(bytes, PEBI, "PiB");
    else                   // EiB
      return formatUnit(bytes, EXBI, "EiB");

#undef formatUnit
}
