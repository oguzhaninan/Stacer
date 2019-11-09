#include "service_tool.h"

#include <QDebug>

Service::Service(const QString &name, const QString description, const bool status, const bool active) :
    name(name),
    description(description),
    status(status),
    active(active)
{ }

QList<Service> ServiceTool::getServicesWithSystemctl()
{    
    QList<Service> services = {};

    try {

        QStringList args = { "list-unit-files", "-t", "service", "-a", "--state=enabled,disabled" };

        QStringList lines = CommandUtil::exec("systemctl", args)
                .split(QChar('\n'))
                .filter(QRegExp("[^@].service"));

        QRegExp sep("\\s+");
        services.reserve(lines.size());
        for (const QString &line : lines)
        {
            // e.g apache2.service          [enabled|disabled]
            QStringList s = line.trimmed().split(sep);

            QString name = s.first().trimmed().replace(".service", "");
            QString description = getServiceDescription(s.first().trimmed());
            bool status = ! s.last().trimmed().compare("enabled");
            bool active = serviceIsActive(s.first().trimmed());

            services.push_back({name, description, status, active});
        }

    } catch(QString &ex) {
        qCritical() << ex;
    }

    return services;
}

QString ServiceTool::getServiceDescription(const QString &serviceName)
{
    QStringList args = { "cat", serviceName };

    QString result("Unknown");

    try {
        QStringList content = CommandUtil::exec("systemctl", args)
                .split(QChar('\n'))
                .filter(QRegExp("^Description"));

        if (content.length() > 0) {
            QStringList desc = content.first().split(QChar('='));
            if (desc.length() > 0)
                result = desc.last();
        }
    } catch (QString &ex) {
        qCritical() << ex;
    }

    return result;
}


bool ServiceTool::serviceIsActive(const QString &serviceName)
{
    QStringList args = { "is-active", serviceName };

    QString result("");

    try {
        result = CommandUtil::exec("systemctl", args);
    } catch(QString &ex) {
        qCritical() << ex;
    }

    return ! result.trimmed().compare("active");
}

bool ServiceTool::serviceIsEnabled(const QString &serviceName)
{
    QStringList args = { "is-enabled", serviceName };

    QString result("");

    try {
        result = CommandUtil::exec("systemctl", args);
    } catch(QString &ex) {
        qCritical() << ex;
    }

    return ! result.trimmed().compare("enabled");
}

bool ServiceTool::changeServiceStatus(const QString &sname, bool status)
{
    try {

        QStringList args = { (status ? "enable" : "disable") , sname };

        CommandUtil::sudoExec("systemctl", args);

        return true;

    } catch(QString &ex) {
        qCritical() << ex;
    }

    return false;
}

bool ServiceTool::changeServiceActive(const QString &sname, bool status)
{
    try {

        QStringList args = { (status ? "start" : "stop") , sname };

        CommandUtil::sudoExec("systemctl", args);

        return true;

    } catch(QString &ex) {
        qCritical() << ex;
    }

    return false;
}
