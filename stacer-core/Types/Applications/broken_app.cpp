#include <algorithm>
#include "broken_app.h"
#include "command_util.h"

inline namespace Types { inline namespace Applications {
/*** begin namespace ***/

BrokenApp::BrokenApp(const Desktop& desktop)
    : m_broken(false), m_ran(nullptr), m_deskfile(desktop)
{

}

BrokenApp::BrokenApp(const Desktop *desktop)
    : m_broken(false), m_ran(nullptr)
{
    Desktop tmp;
    tmp.first = desktop->first;
    tmp.second = desktop->second;
    m_deskfile = std::move(tmp);
}

BrokenApp::~BrokenApp()
{

}

void BrokenApp::run()
{
    if (m_ran != nullptr)
        return;

    const QString       broken     = "ITZ_BROKEN";
    QProcessEnvironment qpe = QProcessEnvironment::systemEnvironment();
    QString             path = qpe.value("PATH", broken);
    QStringList         exec_list;
    QFileInfo           fpt(m_deskfile.second->exec_name); // full path test = fpt
                    /* - If the exec_name is a full path to executable, we good - */
    
    if (fpt.exists() && fpt.isExecutable())
    {
        m_broken = false;
        m_ran = &m_broken;
        return;
    }

    if (path == broken)
        throw DesktopException(DesktopException::ExceptionType::BadEnvironment);

    /*
     * Split the $PATH env variable
     */
    QStringList splitted = path.split(':', QString::SkipEmptyParts);
    QString *str = new QString();
    for (auto path = splitted.begin(); path != splitted.end(); ++path)
    {
	std::fill(str,str+1,QString(*path));

	if (!str->endsWith('/'))
	    str->append('/');

	str->append(m_deskfile.second->exec_name);
	exec_list << *str;
    }

    m_broken = true;
    QFileInfo *qfi = new QFileInfo();
    for (int i=0; i < exec_list.count(); i++)
    {
	    std::fill(qfi, qfi+1, QFileInfo(exec_list[i]));
	    
	    if(qfi->isDir())
		    continue;

	    if(qfi->isFile() && qfi->isExecutable()) {
		    m_broken = false;
		    break;
	    }
    }

    delete qfi;
    delete str;

    // m_broken == whatever
    m_ran = &m_broken;
}

bool BrokenApp::isBroken() const
{
    if (m_ran == nullptr)
        throw DesktopException(DesktopException::ExceptionType::NotParsedError);

    return m_broken;
}

/*** end namespace ***/
} }
