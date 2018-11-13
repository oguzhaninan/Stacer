#ifndef __DESKTOP_H_
#define __DESKTOP_H_

#include <pcrecpp.h>
#include <utility>
#include <string>
#include <QException>
#include <QTextStream>
#include <QString>
#include <QFile>

#include "stacer-core_global.h"

namespace Types
{
    namespace Applications
    {
        
        typedef struct _desktop_file
        {
            QString display_name;
            QString exec_name;
            QString install_prefix;
            QString file_path;
        } DesktopFile;

        typedef std::pair<QFile*, DesktopFile*> Desktop;
        
        /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        
        void STACERCOREFUNCTION_EXPORT loadDesktopFile(const QString& path, Desktop** out_desktop);
    };
};

#endif
