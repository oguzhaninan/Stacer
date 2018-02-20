#ifndef PACKAGE_TOOL_H
#define PACKAGE_TOOL_H

#include <thread>

#include "Utils/command_util.h"
#include "Utils/file_util.h"

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT PackageTool
{
public:
    enum PackageTools {
        APT,        // debian
        DNF,        // fedora
        YUM,        // fedora
        PACMAN,     // arch
        ZYPPER,     // opensuse
        UNKNOWN
    };

public:
    // APT
    static QFileInfoList getDpkgPackageCaches();
    static QStringList getDpkgPackages();
    static bool dpkgRemovePackages(QStringList packages);

    // DNF - YUM
    static QStringList getRpmPackages();
    static bool dnfRemovePackages(QStringList packages);
    static bool yumRemovePackages(QStringList packages);

    // Arch
    static QFileInfoList getPacmanPackageCaches();
    static QStringList getPacmanPackages();
    static bool pacmanRemovePackages(QStringList packages);

    static const PackageTools currentPackageTool;
};

#endif // PACKAGE_TOOL_H
