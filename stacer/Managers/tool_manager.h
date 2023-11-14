#ifndef TOOL_MANAGER_H
#define TOOL_MANAGER_H

#include <Tools/service_tool.h>
#include <Tools/package_tool.h>
#include <Tools/apt_source_tool.h>

class ToolManager
{
public:
    static ToolManager *ins();

    QList<Service> getServices() const;
    QStringList getPackages() const;
    QFileInfoList getPackageCaches() const;

    bool changeServiceStatus(const QString &sname, bool status) const;
    bool changeServiceActive(const QString &sname, bool status) const;
    bool serviceIsActive(const QString &sname) const;
    bool serviceIsEnabled(const QString &sname) const;

    void uninstallPackages(const QStringList &packages);    

    bool checkSourceRepository() const;
    QList<APTSourcePtr> getSourceList() const;
    void removeAPTSource(const APTSourcePtr source);
    void changeAPTStatus(const APTSourcePtr aptSource, const bool status);
    void changeAPTSource(const APTSourcePtr aptSource, const QString newSource);
    void addAPTRepository(const QString &repository, const bool isSource);

private:
    static ToolManager *instance;
};

#endif // TOOL_MANAGER_H
