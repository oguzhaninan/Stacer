#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

#include <type_traits>
#include <utility>
#include <QString>

namespace Types {
/*B*/

static QString BlankString = "";

//
// Base CLasss
//
template<class T, typename DATA, bool ARR = true>
class Command
{
public:
    Command() { m_data = nullptr;}
    virtual   ~Command() { if (m_data) { if (!array_data()) { delete m_data; } else { delete[] m_data; }}}
    
    virtual typename std::add_pointer<T>::type runCommand(T cmd) { return nullptr; }

    
    virtual T convert_data() const { return T(); }
protected:
    DATA    *m_data;

    // is array? as in, is delete[] req'd
    constexpr bool array_data(void) { return ARR; }
    
public:
    const decltype(m_data)    get_data() const { return m_data; }
    
};

//
// popen wrapper
//
class PosixCmd : public Command<QString, unsigned char>
{
    typedef typename std::remove_pointer<decltype(m_data)>::type DATA_TYPE;

public:
    explicit PosixCmd(const bool store_stdout = true);
    PosixCmd(QString cmd_full, const bool store_stdout = true);
    virtual ~PosixCmd();

    virtual void runCommand(QString *cmd = nullptr);
    virtual QString *runCommand(QString cmd);
    
    virtual QString convert_data() const;
    virtual QString* command() const;

private:
    const bool p_stdout;
    QString&   p_cmdstr;

    static constexpr QString& default_blank() { return Types::BlankString; }
};

/*E*/
}

#endif
