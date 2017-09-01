#ifndef SERVICE_TOOL_H
#define SERVICE_TOOL_H

#include <Utils/command_util.h>

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT Service
{
public:
    Service(const QString &name, bool status, bool active);

    QString name;
    bool status;
    bool active;
};

class ServiceTool
{
public:
    ServiceTool();

    static QList<Service> getServicesWithSystemctl();
    static bool serviceIsActive(const QString &serviceName);
    static bool changeServiceStatus(const QString &sname, bool status);
    static bool changeServiceActive(const QString &sname, bool status);
    static bool serviceIsEnabled(const QString &serviceName);
};

#endif // SERVICE_TOOL_H
