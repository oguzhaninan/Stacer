#ifndef SERVICE_TOOL_H
#define SERVICE_TOOL_H

#include <Utils/command_util.h>

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT Service
{
public:
    Service(QString name, bool status, bool active);

    QString name;
    bool status;
    bool active;
};

class ServiceTool
{
public:
    ServiceTool();

    QList<Service> getServicesWithSystemctl();
    bool serviceIsActive(QString serviceName);
    bool changeServiceStatus(QString sname, bool status);
    bool changeServiceActive(QString sname, bool status);
    bool serviceIsEnabled(QString serviceName);
};

#endif // SERVICE_TOOL_H
