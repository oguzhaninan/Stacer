#include "apt_source_tool.h"

bool AptSourceTool::checkSourceRepository()
{
    QDir sourceList(APT_SOURCE_LIST_PATH);

    bool isExists = sourceList.exists();

    return isExists;
}

QFileInfoList AptSourceTool::getSourceList()
{
    QDir aptSourceListDir(APT_SOURCE_LIST_PATH);

    QFileInfoList infoList = aptSourceListDir.entryInfoList({ "*.list", "*.save" },
                                                            QDir::Files,
                                                            QDir::Name);
    return infoList;
}


