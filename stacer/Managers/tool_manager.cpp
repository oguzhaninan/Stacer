#include "tool_manager.h"

ToolManager *ToolManager::instance = NULL;

ToolManager *ToolManager::ins()
{
    if(! instance) {
        instance = new ToolManager;
    }

    return instance;
}

/*
 * Services
 */
QList<Service> ToolManager::getServices() const
{
    return ServiceTool::getServicesWithSystemctl();
}

bool ToolManager::changeServiceStatus(const QString &sname, bool status) const
{
    return ServiceTool::changeServiceStatus(sname, status);
}

bool ToolManager::changeServiceActive(const QString &sname, bool status) const
{
    return ServiceTool::changeServiceActive(sname, status);
}

bool ToolManager::serviceIsActive(const QString &sname) const
{
    return ServiceTool::serviceIsActive(sname);
}

bool ToolManager::serviceIsEnabled(const QString &sname) const
{
    return ServiceTool::serviceIsEnabled(sname);
}

/*
 * Packages
 */
QStringList ToolManager::getPackages() const
{
    switch (PackageTool::currentPackageTool) {
    case PackageTool::PackageTools::APT:
        return PackageTool::getDpkgPackages();
        break;
    case PackageTool::PackageTools::YUM:
    case PackageTool::PackageTools::DNF:
        return PackageTool::getRpmPackages();
        break;
    case PackageTool::PackageTools::PACMAN:
        return PackageTool::getPacmanPackages();
        break;
    default:
        return QStringList();
        break;
    }
}

QStringList ToolManager::getSnapPackages() const
{
    return PackageTool::getSnapPackages();
}

bool ToolManager::uninstallSnapPackages(const QStringList packages)
{
    return PackageTool::snapRemovePackages(packages);
}

QFileInfoList ToolManager::getPackageCaches() const
{
    switch (PackageTool::currentPackageTool) {
    case PackageTool::PackageTools::APT:
        return PackageTool::getDpkgPackageCaches();
        break;
    case PackageTool::PackageTools::YUM:
    case PackageTool::PackageTools::DNF:
        return PackageTool::getPacmanPackageCaches();
        break;
    case PackageTool::PackageTools::PACMAN:
        return PackageTool::getPacmanPackageCaches();
        break;
    default:
        return QFileInfoList();
        break;
    }
}

void ToolManager::uninstallPackages(const QStringList &packages)
{
    switch (PackageTool::currentPackageTool) {
    case PackageTool::PackageTools::APT:
        PackageTool::dpkgRemovePackages(packages);
        break;
    case PackageTool::PackageTools::YUM:
        PackageTool::yumRemovePackages(packages);
        break;
    case PackageTool::PackageTools::DNF:
        PackageTool::dnfRemovePackages(packages);
        break;
    case PackageTool::PackageTools::PACMAN:
        PackageTool::pacmanRemovePackages(packages);
        break;
    default:
        break;
    }
}

/*
 * APT Source
 */
bool ToolManager::checkSourceRepository() const
{
    return AptSourceTool::checkSourceRepository();
}

QList<APTSourcePtr> ToolManager::getSourceList() const
{
    return AptSourceTool::getSourceList();
}

void ToolManager::removeAPTSource(const APTSourcePtr source)
{
    AptSourceTool::removeAPTSource(source);
}

void ToolManager::changeAPTStatus(const APTSourcePtr aptSource, const bool status)
{
    AptSourceTool::changeStatus(aptSource, status);
}

void ToolManager::changeAPTSource(const APTSourcePtr aptSource, const QString newSource)
{
    AptSourceTool::changeSource(aptSource, newSource);
}

void ToolManager::addAPTRepository(const QString &repository, const bool isSource)
{
    AptSourceTool::addRepository(repository, isSource);
}

