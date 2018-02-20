#ifndef AptSourceTool_H
#define AptSourceTool_H

#include "Utils/command_util.h"
#include "Utils/file_util.h"

class AptSourceTool
{
public:
    static bool checkSourceRepository();
    static QFileInfoList getSourceList();
};

#endif // AptSourceTool_H
