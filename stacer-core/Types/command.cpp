#include "command.hpp"

inline namespace Types {

PosixCmd::PosixCmd(const bool store_stdout)
 : Command<QString, unsigned char>(), p_stdout(store_stdout), p_cmdstr(default_blank())
{
    m_data = new DATA_TYPE[4096];
    std::fill_n(m_data, 4096, '\0');
}

PosixCmd::PosixCmd(QString cmd_full, const bool store_stdout)
 : Command<QString, unsigned char>(), p_stdout(store_stdout), p_cmdstr(cmd_full)
{
    m_data = new DATA_TYPE[4096];
    std::fill_n(m_data, 4096, '\0');
}

PosixCmd::~PosixCmd()
{

}

void PosixCmd::runCommand(QString *cmd)
{
    FILE *fp = nullptr;

    if (cmd == nullptr)
    {
        fp = popen(p_cmdstr.toStdString().c_str(), "r");
    }
    else
    {
        fp = popen(cmd->toStdString().c_str(), "r");
    }

    if (fp == nullptr)
        return;

    pclose(fp);
}

QString* PosixCmd::runCommand(QString cmd)
{
    FILE *fp = popen(cmd.toStdString().c_str(), "r");
    int offset = 0;
    int addy   = 0;

    do
    {
        fgets(reinterpret_cast<char*>(m_data+offset), 80, fp);
        addy = ftell(fp);
        offset += 80;
    }
    while(feof(fp) == 0);
    \
    pclose(fp);
    return new QString(convert_data());
}

QString PosixCmd::convert_data() const
{
    QByteArray  convy((char*)m_data, 4096);
    QString final(convy);

    return std::move(final);
}

QString* PosixCmd::command() const
{
    return &p_cmdstr;
}

/*namespace over dude*/
}
