#include "broken_app.h"
#include "desktop.h"

void STACERCOREFUNCTION_EXPORT Types::Applications::loadDesktopFile(const QString& path, Desktop** out_desktop)
{
    Desktop tmp;

    if (!QFile::exists(path))
        throw QException();

    tmp.first = new QFile(path);
    tmp.first->open(QIODevice::ReadOnly | QIODevice::Text);
    tmp.second = new DesktopFile();
    
    size_t a{},b{};
    std::string buffer;
    QTextStream in(tmp.first);
    while (!in.atEnd())
    {
            QString cur_line = in.readLine();
            buffer.clear();

            if (a <= 0)
            {
                using namespace pcrecpp;
                RE name_re("^Name=(.*)");
                name_re.FullMatch(cur_line.toStdString(), &buffer);
                tmp.second->display_name = QString::fromStdString(buffer);
            	a = tmp.second->display_name.length();
            }
            else if (b <= 0)
            {
                using namespace pcrecpp;
                RE exec_re("^Exec=(\\w*)\\s?");
                exec_re.FullMatch(cur_line.toStdString(), &buffer);
                tmp.second->exec_name = QString::fromStdString(buffer);
            	b = tmp.second->exec_name.length();
            }
    }

    *out_desktop = new Desktop(tmp);
}

bool STACERCOREFUNCTION_EXPORT Types::Applications::loadAndCheckDesktopFile(const QString& path)
{
    Desktop *to_check = nullptr;
    bool    retval    = false;

    loadDesktopFile(path, &to_check);

    BrokenApp broke_or_not(to_check);
    try
    {
        try
        {
            retval = broke_or_not.isBroken();
        }
        catch (DesktopException &de)
        {
            broke_or_not.run();
        }

        retval = broke_or_not.isBroken();
    }
    catch (DesktopException &de)
    {
        if (de.type() != DesktopException::ExceptionType::BadEnvironment)
        {
            broke_or_not.run();
            retval = broke_or_not.isBroken();
        }
        else
        {
            throw QUnhandledException();
        }
    }

    delete to_check;
    return retval;
}
