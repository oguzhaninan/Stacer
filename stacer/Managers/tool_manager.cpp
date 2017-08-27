#include "tool_manager.h"

ToolManager *ToolManager::_instance = NULL;

ToolManager *ToolManager::ins()
{
    if(_instance == NULL)
        _instance = new ToolManager;

    return _instance;
}

ToolManager::ToolManager() :
    st(new ServiceTool),
    pt(new PackageTool)
{

}

/******************
 * SERVICES
 *****************/
QList<Service> ToolManager::getServices()
{
    return st->getServicesWithSystemctl();
}

bool ToolManager::changeServiceStatus(QString sname, bool status)
{
    return st->changeServiceStatus(sname, status);
}

bool ToolManager::changeServiceActive(QString sname, bool status)
{
    return st->changeServiceActive(sname, status);
}

bool ToolManager::serviceIsActive(QString sname)
{
    return st->serviceIsActive(sname);
}

bool ToolManager::serviceIsEnabled(QString sname)
{
    return st->serviceIsEnabled(sname);
}

/******************
 * PACKAGES
 *****************/
QStringList ToolManager::getPackages()
{
    switch (pt->getCurrentPackageTool()) {
    case PackageTool::PackageTools::APT:
        return pt->getDpkgPackages();
        break;
    case PackageTool::PackageTools::YUM:
    case PackageTool::PackageTools::DNF:
        return pt->getRpmPackages();
        break;
    case PackageTool::PackageTools::PACMAN:
        return pt->getPacmanPackages();
        break;
    default:
        return QStringList();
        break;
    }
}

QFileInfoList ToolManager::getPackageCaches()
{
    switch (pt->getCurrentPackageTool()) {
    case PackageTool::PackageTools::APT:
        return pt->getDpkgPackageCaches();
        break;
    case PackageTool::PackageTools::YUM:
    case PackageTool::PackageTools::DNF:
        return pt->getPacmanPackageCaches();
        break;
    case PackageTool::PackageTools::PACMAN:
        return pt->getPacmanPackageCaches();
        break;
    default:
        return QFileInfoList();
        break;
    }
}

void ToolManager::uninstallPackages(QStringList packages)
{
    uninstallStarted();

    switch (pt->getCurrentPackageTool()) {
    case PackageTool::PackageTools::APT:
        pt->dpkgRemovePackages(packages);
        break;
    case PackageTool::PackageTools::YUM:
        pt->yumRemovePackages(packages);
        break;
    case PackageTool::PackageTools::DNF:
        pt->dnfRemovePackages(packages);
        break;
    case PackageTool::PackageTools::PACMAN:
        pt->pacmanRemovePackages(packages);
        break;
    }

    uninstallFinished();
}

