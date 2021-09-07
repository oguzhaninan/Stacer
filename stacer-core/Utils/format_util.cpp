#include "format_util.h"

#include <QString>

QString FormatUtil::formatBytes(const quint64 &bytes)
{
#define formatUnit(v, u, t) QString().asprintf("%.1f %s", ((double) v / (double) u), t)
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
    else                   // TiB
      return formatUnit(bytes, TEBI, "TiB");
#undef formatUnit
}
