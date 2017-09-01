#include "process.h"

Process::Process()
{

}

pid_t Process::getPid() const
{
    return pid;
}

void Process::setPid(const pid_t &value)
{
    pid = value;
}

quint64 Process::getRss() const
{
    return rss;
}

void Process::setRss(const quint64 &value)
{
    rss = value;
}

double Process::getPmem() const
{
    return pmem;
}

void Process::setPmem(const double &value)
{
    pmem = value;
}

quint64 Process::getVsize() const
{
    return vsize;
}

void Process::setVsize(const quint64 &value)
{
    vsize = value;
}

QString Process::getUname() const
{
    return uname;
}

void Process::setUname(const QString &value)
{
    uname = value;
}

double Process::getPcpu() const
{
    return pcpu;
}

void Process::setPcpu(const double &value)
{
    pcpu = value;
}

QString Process::getCmd() const
{
    return cmd;
}

void Process::setCmd(const QString &value)
{
    cmd = value;
}

QString Process::getStartTime() const
{
    return startTime;
}

void Process::setStartTime(const QString &value)
{
    startTime = value;
}

QString Process::getState() const
{
    return state;
}

void Process::setState(const QString &value)
{
    state = value;
}

QString Process::getGroup() const
{
    return group;
}

void Process::setGroup(const QString &value)
{
    group = value;
}

int Process::getNice() const
{
    return nice;
}

void Process::setNice(const int &value)
{
    nice = value;
}

QString Process::getCpuTime() const
{
    return cpuTime;
}

void Process::setCpuTime(const QString &value)
{
    cpuTime = value;
}

QString Process::getSession() const
{
    return session;
}

void Process::setSession(const QString &value)
{
    session = value;
}

QString Process::getSeat() const
{
    return seat;
}

void Process::setSeat(const QString &value)
{
    seat = value;
}
