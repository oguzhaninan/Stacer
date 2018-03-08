#ifndef AptSourceTool_H
#define AptSourceTool_H

#include "Utils/command_util.h"
#include "Utils/file_util.h"
#include <QSharedPointer>

#define APT_SOURCES_LIST_D_PATH "/etc/apt/sources.list.d"
#define APT_SOURCES_LIST_PATH "/etc/apt/sources.list"

//          isSource options uri distribution componentes
// example "deb [arch=amd64] http://packages.microsoft.com/repos/vscode stable main"

class APTSource {
public:
    QString filePath;
    bool isSource;
    QString options;
    QString uri;
    QString distribution;
    QString components;

    QString source;
    bool isActive;
};

typedef QSharedPointer<APTSource> APTSourcePtr;

class AptSourceTool
{
public:
    static bool checkSourceRepository();
    static QList<APTSourcePtr> getSourceList();
    static void removeAPTSource(const APTSourcePtr aptSource);
    static void changeStatus(const APTSourcePtr aptSource, const bool status);
    static void changeSource(const APTSourcePtr aptSource, const QString newSource);
    static void addRepository(const QString &repository, const bool isSource);
};

#endif // AptSourceTool_H
