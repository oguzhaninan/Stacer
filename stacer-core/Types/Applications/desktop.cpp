#include "desktop.h"

void STACERCOREFUNCTION_EXPORT Types::Applications::loadDesktopFile(const QString& path, Desktop** out_desktop)
{
    Desktop tmp;

    if (!QFile::exists(path))
        throw QException();

    tmp.first = new QFile(path);
    tmp.first->open(QIODevice::ReadOnly | QIODevice::Text);
    tmp.second = new DesktopFile();
    
    size_t a,b;
    std::string buffer;
    QTextStream in(tmp.first);
    while (!in.atEnd())
    {
            QString cur_line = in.readLine();
            a = tmp.second->display_name.length();
            b = tmp.second->exec_name.length();
            
            if (a <= 0)
            {
                using namespace pcrecpp;
                RE name_re("^Name=(.*)");
                name_re.FullMatch(cur_line.toStdString(), &buffer);
                tmp.second->display_name = QString::fromStdString(buffer);
            }
            else if (b <= 0)
            {
                using namespace pcrecpp;
                RE exec_re("^Exec=(\\w*)\\s?");
                exec_re.FullMatch(cur_line.toStdString(), &buffer);
                tmp.second->exec_name = QString::fromStdString(buffer);
            }
    }
    if (in.atEnd() && (a | b == 0))
    {
        throw QException();
    }

    *out_desktop = new Desktop(tmp);
}
