#ifndef BROKEN_APP_H
#define BROKEN_APP_H

#include <QRunnable>
#include <QProcessEnvironment>
#include <QFileInfo>
#include <QException>

#include "desktop.h"

namespace Types
{
    namespace Applications
    {
        class STACERCORESHARED_EXPORT BrokenApp : public QRunnable
        {
        public:
            explicit BrokenApp(const Desktop& desktop);
            explicit BrokenApp(const Desktop* desktop);
            virtual ~BrokenApp();

            virtual void run();

            bool isBroken() const;

        private:
             bool   m_broken;
             bool  *m_ran;
             Desktop m_deskfile;
        };
    }
}


#endif // BROKEN_APP_H
