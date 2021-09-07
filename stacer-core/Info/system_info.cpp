#include "system_info.h"

#include <QRegularExpression>
#include <QObject>

SystemInfo::SystemInfo()
{
    QString unknown(QObject::tr("Unknown"));
    QString model;
    QString speed;

    try{
        QStringList lines = CommandUtil::exec("bash",{"-c", LSCPU_COMMAND}).split('\n');  //run command in English language (guaratee same behaviour across languages)

        QRegularExpression regexp("\\s+");
        QString space(" ");

        auto filterModel = lines.filter(QRegularExpression("^Model name"));
        QString modelLine = filterModel.isEmpty() ? "error missing model:error missing model" : filterModel.first();
        auto filterSpeed = lines.filter(QRegularExpression("^CPU max MHz"));
        QString speedLine = "error:0.0";
        if (filterSpeed.isEmpty())
        {
            // fallback to CPU MHz
            filterSpeed = lines.filter(QRegularExpression("^CPU MHz"));
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

QString SystemInfo::getUsername() const
{
    return username;
}

QString SystemInfo::getHostname()
{
    return QSysInfo::machineHostName();
}

QStringList SystemInfo::getUserList()
{
    QStringList passwdUsers = FileUtil::readListFromFile("/etc/passwd");
    QStringList users;

    for(QString &row: passwdUsers) {
        users.append(row.split(":").at(0));
    }

    return users;
}

QStringList SystemInfo::getGroupList()
{
    QStringList groupFile = FileUtil::readListFromFile("/etc/group");
    QStringList groups;

    for(QString &row: groupFile) {
        groups.append(row.split(":").at(0));
    }

    return groups;
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
