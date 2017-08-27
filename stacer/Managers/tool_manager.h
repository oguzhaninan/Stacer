#ifndef TOOL_MANAGER_H
#define TOOL_MANAGER_H

#include <QObject>
#include <QDebug>

#include <Tools/service_tool.h>
#include <Tools/package_tool.h>

class ToolManager : public QObject
{
    Q_OBJECT

public:
    static ToolManager *ins();

public:
    QList<Service> getServices();
    QStringList getPackages();

    QFileInfoList getPackageCaches();
public slots:
    bool changeServiceStatus(QString sname, bool status);
    bool changeServiceActive(QString sname, bool status);
    bool serviceIsActive(QString sname);
    bool serviceIsEnabled(QString sname);
    void uninstallPackages(QStringList packages);

signals:
    void uninstallFinished();
    void uninstallStarted();

private:
    ToolManager();
    static ToolManager *_instance;

    ServiceTool *st;
    PackageTool *pt;
};

#endif // TOOL_MANAGER_H
