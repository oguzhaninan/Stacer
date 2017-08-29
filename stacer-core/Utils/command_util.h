#ifndef COMMAND_UTIL_H
#define COMMAND_UTIL_H

#include <QStringList>

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT CommandUtil
{
public:
    static QString sudoExec(QString cmd, QStringList args = {});
    static QString exec(QString cmd, QStringList args = {});
    static bool isExecutable(QString cmd);
private:
    CommandUtil();
};

#endif // COMMAND_UTIL_H
