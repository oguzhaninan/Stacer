#include "command.hpp"

inline namespace Types {

PosixCmd::PosixCmd(const bool store_stdout)
 : Command<QString, unsigned char>()
{

}

PosixCmd::PosixCmd(QString cmd_full, const bool store_stdout)
{

}

PosixCmd::~PosixCmd()
{

}

QString* PosixCmd::runCommand(QString cmd)
{
    return nullptr;
}

QString PosixCmd::convert_data() const {
    QString final;

    return std::move(final);
}

void PosixCmd::init() {

}

/*namespace over dude*/
}
