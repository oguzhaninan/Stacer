#include "package_tool.h"

#include <QRegularExpression>
#include <QDebug>

const PackageTool::PackageTools PackageTool::currentPackageTool =
        CommandUtil::isExecutable("apt-get") ? PackageTool::APT :
        CommandUtil::isExecutable("dnf")     ? PackageTool::DNF :
        CommandUtil::isExecutable("yum")     ? PackageTool::YUM :
        CommandUtil::isExecutable("pacman")  ? PackageTool::PACMAN :
        CommandUtil::isExecutable("zypper")  ? PackageTool::ZYPPER :
                                               PackageTool::UNKNOWN;

/***********
 * DPKG
 ***********/
QFileInfoList PackageTool::getDpkgPackageCaches()
{
    QDir caches("/var/cache/apt/archives/");

    return caches.entryInfoList(QDir::Files);
}

QStringList PackageTool::getDpkgPackages()
{
    QStringList packageList = {};

    try {
        packageList = CommandUtil::exec("bash", {"-c", "dpkg --get-selections 2> /dev/null"})
                .trimmed()
                .split('\n')
                .filter(QRegularExpression("\\s+install$"));

        for (int i = 0; i < packageList.count(); ++i)
            packageList[i] = packageList.at(i).split(QRegularExpression("\\s+")).first();

    } catch(QString &ex) {
        qCritical() << ex;
    }

    return packageList;
}

bool PackageTool::dpkgRemovePackages(QStringList packages)
{
    try {
        packages.insert(0, "remove");
        packages.insert(1, "-y");

        CommandUtil::sudoExec("apt-get", packages);

        return true;

    } catch(QString &ex) {
        qCritical() << ex;
    }

    return false;
}

/**********
 * RPM
 **********/
QStringList PackageTool::getRpmPackages()
{
    QStringList packageList = {};

    try {
        packageList = CommandUtil::exec("bash", {"-c", "rpm -qa 2> /dev/null"})
                .trimmed()
                .split('\n');

    } catch(QString &ex) {
        qCritical() << ex;
    }

    return packageList;
}

bool PackageTool::dnfRemovePackages(QStringList packages)
{
    try {
        packages.insert(0, "remove");
        packages.insert(1, "-y");

        CommandUtil::sudoExec("dnf", packages);

        return true;

    } catch(QString &ex) {
        qCritical() << ex;
    }

    return false;
}

bool PackageTool::yumRemovePackages(QStringList packages)
{
    try {
        packages.insert(0, "remove");
        packages.insert(1, "-y");

        CommandUtil::sudoExec("yum", packages);

        return true;

    } catch(QString &ex) {
        qCritical() << ex;
    }

    return false;
}

/**********
 * PACMAN
 **********/
QFileInfoList PackageTool::getPacmanPackageCaches()
{
    QDir caches("/var/cache/pacman/pkg/");

    return caches.entryInfoList(QDir::Files);
}

QStringList PackageTool::getPacmanPackages()
{
    QStringList packageList = {};

    try {
        packageList = CommandUtil::exec("bash", {"-c", "pacman -Q 2> /dev/null"})
                .trimmed()
                .split('\n');

        for (int i = 0; i < packageList.count(); ++i)
            packageList[i] = packageList.at(i).split(QRegularExpression("\\s+")).first();

    } catch(QString &ex) {
        qCritical() << ex;
    }

    return packageList;
}

bool PackageTool::pacmanRemovePackages(QStringList packages)
{
    try {
        packages.push_back("--noconfirm");
        packages.push_back("-R");

        CommandUtil::sudoExec("pacman", packages);

        return true;

    } catch(QString &ex) {
        qCritical() << ex;
    }

    return false;
}

/**********
 * SNAP
 **********/
QStringList PackageTool::getSnapPackages()
{
    QStringList packageList = {};

    if (CommandUtil::isExecutable("snap")) {
        try {
            packageList = CommandUtil::exec("snap", {"list"})
                    .trimmed()
                    .split('\n');

            packageList.removeFirst(); // remove titles e.g name, version

            for (int i = 0; i < packageList.count(); ++i)
                packageList[i] = packageList.at(i).split(QRegularExpression("\\s+")).first();

        } catch (QString &ex) {
            qCritical() << ex;
        }
    }

    return packageList;
}

bool PackageTool::snapRemovePackages(QStringList packages)
{
    try {
        packages.insert(0, "remove");
        qDebug() << packages;

        CommandUtil::sudoExec("snap", packages);

        return true;

    } catch(QString &ex) {
        qCritical() << ex;
    }

    return false;
}
