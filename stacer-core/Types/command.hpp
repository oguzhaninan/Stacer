#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

#include <type_traits>
#include <utility>
#include <QString>

namespace Types {
/*B*/

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

    constexpr bool array_data(void) { return ARR; }
    
public:
    const decltype(m_data)    get_data() const { return m_data; }
    
};

class PosixCmd : public Command<QString, unsigned char>
{
public:
    explicit PosixCmd(const bool store_stdout = true);
    PosixCmd(QString cmd_full, const bool store_stdout = true);
    virtual ~PosixCmd();

    virtual QString *runCommand(QString cmd);
    
    virtual QString convert_data() const;
private:
    void init();
};

/*E*/
}

#endif
