#include "system_info.h"

#include <QObject>
#include <iostream>

SystemInfo::SystemInfo()
{
    QString unknown(QObject::tr("Unknown"));
    QString model = nullptr;
    QString speed = nullptr;

    try{
        QStringList lines = CommandUtil::exec("bash",{"-c", LSCPU_COMMAND}).split('\n');  //run command in English language (guaratee same behaviour across languages)

        QRegExp regexp("\\s+");
        QString space(" ");

        auto filterModel = lines.filter(QRegExp("^Model name"));
        QString modelLine = filterModel.isEmpty() ? "error missing model:error missing model" : filterModel.first();
        auto filterSpeed = lines.filter(QRegExp("^CPU max MHz"));
        QString speedLine = "error:0.0";
        if (filterSpeed.isEmpty())
        {
            // fallback to CPU MHz
            filterSpeed = lines.filter(QRegExp("^CPU MHz"));
            speedLine = filterSpeed.isEmpty() ? speedLine : filterSpeed.first();
        }

        model = modelLine.split(":").last();
        speed = speedLine.split(":").last();

        model = model.contains('@') ? model.split("@").first() : model; // intel : AMD
        speed = QString::number(speed.toDouble()/1000.0) + "GHz";

        this->cpuModel = model.trimmed().replace(regexp, space);
        this->cpuSpeed = speed.trimmed().replace(regexp, space);
    } catch(QString &ex) {
        this->cpuModel = unknown;
        this->cpuSpeed = unknown;
    }

    CpuInfo ci;
    this->cpuCore = QString::number(ci.getCpuPhysicalCoreCount());

    GpuInfo gi;
    this->sucessGPUInfo = gi.finding_GPU_info;
    if(gi.finding_GPU_info){
        this->gpuNode = gi.getGPUPhysicalCoreCount(gi);
        this->gpuNodeString = QString::number(this->gpuNode);
        this->GPUDriverVersion = gi.getGPUDriverVersion(gi);
        this->GPUAcceleratorVersion = gi.getGPUAcceleratorVersion(gi);
        this->GPUTotalMemory = gi.getGPUMemoryTotal(gi);
        this->gpuNames = gi.getGPUName(gi);
    }



    // get username
    QString name = qgetenv("USER");

    if (name.isEmpty())
        name = qgetenv("USERNAME");

    try {
        if (name.isEmpty())
            name = CommandUtil::exec("whoami").trimmed();
    } catch (const QString &ex) {
        qCritical() << ex;
    }

   this->username = name;
}

std::vector<double> SystemInfo::getGPUTotalMemory() const
{
    return GPUTotalMemory;
}

QString SystemInfo::getGPUCoreString() const
{
    return gpuNodeString;
}

QString SystemInfo::getGPUDriverVersion() const
{
    return GPUDriverVersion;
}

QStringList SystemInfo::getGPUName() const
{
    return gpuNames;
}

QString SystemInfo::getGPUAcceleratorVersion() const
{
    return GPUAcceleratorVersion;
}

int SystemInfo::getGPUCore() const
{
    return gpuNode;
}

QString SystemInfo::getUsername() const
{
    return username;
}

QString SystemInfo::getHostname() const
{
    return QSysInfo::machineHostName();
}

QStringList SystemInfo::getUserList() const
{
    QStringList passwdUsers = FileUtil::readListFromFile("/etc/passwd");
    QStringList users;

    for(QString &row: passwdUsers) {
        users.append(row.split(":").at(0));
    }

    return users;
}

QStringList SystemInfo::getGroupList() const
{
    QStringList groupFile = FileUtil::readListFromFile("/etc/group");
    QStringList groups;

    for(QString &row: groupFile) {
        groups.append(row.split(":").at(0));
    }

    return groups;
}

QString SystemInfo::getPlatform() const
{
    return QString("%1 %2")
            .arg(QSysInfo::kernelType())
            .arg(QSysInfo::currentCpuArchitecture());
}

QString SystemInfo::getDistribution() const
{
    return QSysInfo::prettyProductName();
}

QString SystemInfo::getKernel() const
{
    return QSysInfo::kernelVersion();
}

QString SystemInfo::getCpuModel() const
{
    return this->cpuModel;
}

QString SystemInfo::getCpuSpeed() const
{
    return this->cpuSpeed;
}

QString SystemInfo::getCpuCore() const
{
    return this->cpuCore;
}

QFileInfoList SystemInfo::getCrashReports() const
{
    QDir reports("/var/crash");

    return reports.entryInfoList(QDir::Files);
}

QFileInfoList SystemInfo::getAppLogs() const
{
    QDir logs("/var/log");

    return logs.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
}

QFileInfoList SystemInfo::getAppCaches() const
{
    QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    QDir caches(homePath + "/.cache");

    return caches.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
}
