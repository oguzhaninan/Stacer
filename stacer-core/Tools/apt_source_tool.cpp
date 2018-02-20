#include "apt_source_tool.h"

bool AptSourceTool::checkSourceRepository()
{
    QDir sourceList("/etc/apt/sources.list.d");

    bool isExists = sourceList.exists();

    return isExists;
}

QFileInfoList AptSourceTool::getSourceList()
{
    QDir aptSourceList();
}


