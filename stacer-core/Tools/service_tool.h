#ifndef SERVICE_TOOL_H
#define SERVICE_TOOL_H

#include <QObject>
#include <QDebug>

#include <Utils/command_util.h>

#include "stacer-core_global.h"
class STACERCORESHARED_EXPORT Service {

public:
    Service(QString name, bool status, bool active) {
        this->name = name;
        this->status = status;
        this->active = active;
    }

    QString name;
    bool status;
    bool active;
};

class ServiceTool : public QObject
{
    Q_OBJECT

public:
    ServiceTool();

    QList<Service> getServicesWithSystemctl();
    bool serviceIsActive(QString serviceName);
    bool changeServiceStatus(QString sname, bool status);
    bool changeServiceActive(QString sname, bool status);
    bool serviceIsEnabled(QString serviceName);
private:

};

#endif // SERVICE_TOOL_H
