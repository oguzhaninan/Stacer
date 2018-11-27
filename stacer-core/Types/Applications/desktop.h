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
        
        void STACERCOREFUNCTION_EXPORT loadDesktopFile(const QString& path, Desktop** out_desktop);

        // TRUE If broken
        bool STACERCOREFUNCTION_EXPORT loadAndCheckDesktopFile(const QString& path);
    };
};

#endif
