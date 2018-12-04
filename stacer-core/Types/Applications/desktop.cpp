#include "broken_app.h"
#include "desktop.h"

void STACERCOREFUNCTION_EXPORT Types::Applications::freeDesktop(Desktop *to_free)
{
    QFile       *chk1 = q_check_ptr<QFile>(to_free->first);
    DesktopFile *chk2 = q_check_ptr<DesktopFile>(to_free->second);
    
    if (chk1 != nullptr)
        delete to_free->first;
    
    if (chk2 != nullptr)
        delete to_free->second;
    
    delete to_free;
}

void STACERCOREFUNCTION_EXPORT Types::Applications::loadDesktopFile(const QString& path, Desktop** out_desktop)
{
    Desktop tmp;
    QFileInfo file_info(path);

    if (!QFile::exists(path))
        throw QException();

    tmp.first = new QFile(path);
    tmp.first->open(QIODevice::ReadOnly | QIODevice::Text);
    tmp.second = new DesktopFile();
    
    // Count the lines
    size_t a{},b{},lines{},lines2{};
    QTextStream counter(tmp.first);
    while (!counter.atEnd())
    {
        counter.readLine();
        ++lines;
    }
    tmp.first->seek(0);
    
    // Parse the file
    std::string buffer;
    QTextStream in(tmp.first);
    while (!in.atEnd())
    {
            // Both regexes are solved
            if (a*b != 0)
                break;
            
            QString cur_line = in.readLine();
            ++lines2;
            buffer.clear();
            
            // Reread .desktop if Exec not found but we're almost @ end
            if ((a != 0 && b <= 0) && 1+lines2 == lines)
            {
                tmp.first->seek(0);
                in.seek(0);
                continue;
            }
            
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
                RE exec_re("Exec=([\\/\\w\\-\\.]*\\w*)\\s?");
                exec_re.FullMatch(cur_line.toStdString(), &buffer);
                tmp.second->exec_name = QString::fromStdString(buffer);
            	b = tmp.second->exec_name.length();
            }
    }
    
    // populate last two fields
    tmp.second->install_prefix = file_info.path();
    tmp.second->file_path = path;

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

    freeDesktop(to_check);
    return retval;
}

qint64 STACERCOREFUNCTION_EXPORT Types::Applications::listBrokenApps(const QStringList& list_all, QStringList *list_broke)
{
    qint64                   count_broken = 0;
    QList<QString>           broke_list;
    
    for (const auto& strpath : list_all)
    {
        if (loadAndCheckDesktopFile(strpath) == true)
        {
            count_broken += 1;
            broke_list.append(strpath);
        }
    }
    /************* Output it!!! ********************/
    QStringList              pre_lb = broke_list;
    std::fill(list_broke, list_broke+1, pre_lb);
    
    return count_broken;
}

