#include "system_info.h"

SystemInfo::SystemInfo() :
    info(new QSysInfo)
{
    QStringList lines = FileUtil::readListFromFile(PROC_CPUINFO)
            .filter(QRegExp("^model name"));

    if (! lines.isEmpty()) {
        QStringList model = lines.first().split(":").at(1).split("@");

        if ( model.count() > 1) {
            this->cpuModel = model.at(0).trimmed().replace(QRegExp("\\s+"), " ");
            this->cpuSpeed = model.at(1).trimmed().replace(QRegExp("\\s+"), " ");
        }
    }
    else {
        this->cpuModel = tr("Unknown");
        this->cpuSpeed = tr("Unknown");
    }

    CpuInfo ci;
    this->cpuCore = QString::number(ci.getCpuCoreCount());

    // get username
    QString name = qgetenv("USER");

    if (name.isEmpty())
        name = qgetenv("USERNAME");

    try {
        if (name.isEmpty())
            name = CommandUtil::exec("whoami").trimmed();
    } catch (QString &ex) {
        qCritical() << ex;
    }

    username = name;
}

QString SystemInfo::getUsername() const
{
    return username;
}

QString SystemInfo::getHostname()
{
    return QSysInfo::machineHostName();
}

QString SystemInfo::getPlatform()
{
    return QString("%1 %2")
            .arg(QSysInfo::kernelType())
            .arg(QSysInfo::currentCpuArchitecture());
}

QString SystemInfo::getDistribution()
{
    return QSysInfo::prettyProductName();
}

QString SystemInfo::getKernel()
{
    return QSysInfo::kernelVersion();
}

QString SystemInfo::getCpuModel()
{
    return this->cpuModel;
}

QString SystemInfo::getCpuSpeed()
{
    return this->cpuSpeed;
}

QString SystemInfo::getCpuCore()
{
    return this->cpuCore;
}

QFileInfoList SystemInfo::getCrashReports()
{
    QDir reports("/var/crash");

    return reports.entryInfoList(QDir::Files);
}

QFileInfoList SystemInfo::getAppLogs()
{
    QDir logs("/var/log");

    return logs.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
}

QFileInfoList SystemInfo::getAppCaches()
{
    QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    QDir caches(homePath + "/.cache");

    return caches.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
}
