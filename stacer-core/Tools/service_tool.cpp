#include "service_tool.h"

#include <QDebug>

Service::Service(const QString &name, bool status, bool active) :
    name(name),
    status(status),
    active(active)
{

}

ServiceTool::ServiceTool()
{

}

QList<Service> ServiceTool::getServicesWithSystemctl()
{    
    QList<Service> services = {};

    try {

        QStringList args = { "list-unit-files", "-t", "service", "-a", "--state=enabled,disabled" };

        QStringList lines = CommandUtil::exec("systemctl", args)
                .split(QChar('\n'))
                .filter(QRegExp("[^@].service"));

        QRegExp sep("\\s+");
        for (const QString &line : lines)
        {
            // e.g apache2.service          [enabled|disabled]
            QStringList s = line.trimmed().split(sep);

            QString name = s.first().trimmed().replace(".service", "");
            bool status = ! s.last().trimmed().compare("enabled");
            bool active = serviceIsActive(s.first().trimmed());

            Service service(name, status, active);

            services << service;
        }

    } catch(QString &ex) {
        qCritical() << ex;
    }

    return services;
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
