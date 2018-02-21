#ifndef AptSourceTool_H
#define AptSourceTool_H

#include "Utils/command_util.h"
#include "Utils/file_util.h"
#include <QSharedPointer>

#define APT_SOURCES_LIST_D_PATH "/etc/apt/sources.list.d"
#define APT_SOURCES_LIST_PATH "/etc/apt/sources.list"

class APTSource {
public:
    QString filePath;
    QString source;
    bool isSource;
    bool isActive;
};

typedef QSharedPointer<APTSource> APTSourcePtr;

class AptSourceTool
{
public:
    static bool checkSourceRepository();
    static QList<APTSourcePtr> getSourceList();
};

#endif // AptSourceTool_H
