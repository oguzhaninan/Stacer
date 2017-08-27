#ifndef PACKAGE_TOOL_H
#define PACKAGE_TOOL_H

#include <QObject>
#include <QDebug>
#include <thread>

#include "Utils/command_util.h"
#include "Utils/file_util.h"

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT PackageTool : public QObject
{
    Q_OBJECT

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
    PackageTool();

public slots:
    // APT
    QFileInfoList getDpkgPackageCaches();
    QStringList getDpkgPackages();
    bool dpkgRemovePackages(QStringList packages);

    // DNF - YUM
    QStringList getRpmPackages();
    bool dnfRemovePackages(QStringList packages);
    bool yumRemovePackages(QStringList packages);

    // Arch
    QFileInfoList getPacmanPackageCaches();
    QStringList getPacmanPackages();
    bool pacmanRemovePackages(QStringList packages);

    PackageTools getCurrentPackageTool() const;
private:
    PackageTools currentPackageTool;
};

#endif // PACKAGE_TOOL_H
