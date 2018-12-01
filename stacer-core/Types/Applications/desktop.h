#ifndef __DESKTOP_H_
#define __DESKTOP_H_

#include <algorithm>
#include <pcrecpp.h>
#include <utility>
#include <string>
#include <QException>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QFile>

#include "stacer-core_global.h"

namespace Types
{
    namespace Applications
    {
        class DesktopException : public QException
        {
        public:
            enum class ExceptionType
            {
                LoadingError,
                NotParsedError,
                BadEnvironment
            };
        private:
            typedef ExceptionType Type;
        public:
            explicit DesktopException(Type etype)
                : m_type(etype)
            {}
            virtual ~DesktopException()
            {}
            const Type& type() const
            { return m_type; }
        private:
            const ExceptionType m_type;
        };

        typedef struct _desktop_file
        {
            QString display_name;
            QString exec_name;
            QString install_prefix;
            QString file_path;
        } DesktopFile;

        typedef std::pair<QFile*, DesktopFile*> Desktop;
        
        /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        
        void STACERCOREFUNCTION_EXPORT freeDesktop(Desktop *to_free);
        
        void STACERCOREFUNCTION_EXPORT loadDesktopFile(const QString& path, Desktop** out_desktop);

        // TRUE If broken
        bool STACERCOREFUNCTION_EXPORT loadAndCheckDesktopFile(const QString& path);
        
        // Returns count of broken apps 
        // // list_broke is ptr to QStringList that will be modified
        qint64 STACERCOREFUNCTION_EXPORT listBrokenApps(const QStringList& list_all, QStringList *list_broke);
    };
};

#endif
