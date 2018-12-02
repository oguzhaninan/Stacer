#ifndef STACERCORE_GLOBAL_H
#define STACERCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(STACERCORE_LIBRARY)
#  define STACERCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define STACERCORESHARED_EXPORT Q_DECL_IMPORT
#endif

#if defined(GNU_SOURCE)
#  define STACERCOREFUNCTION_EXPORT __attribute__((visibility("default")))
#  define STACERCORE_NOEXPORT  __attribute__((visibility("hidden")))
#endif

#endif // STACERCORE_GLOBAL_H
