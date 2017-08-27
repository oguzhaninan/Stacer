#include "process_info.h"

ProcessInfo::ProcessInfo()
{

}

void ProcessInfo::updateProcesses()
{
    processList.clear();

    try {

        QStringList columns = { "pid", "rss", "pmem", "vsize", "uname", "pcpu", "start_time",
                                "state", "group", "nice", "cputime", "session", "seat", "cmd"};

        QStringList lines = CommandUtil::exec("ps", {"ax", "-eo", columns.join(","), "--no-headings"})
                .trimmed()
                .split("\n");

        if (! lines.isEmpty()) {
            foreach (QString line, lines) {
                QStringList procLine = line.trimmed().split(QRegExp("\\s+"));

                if (procLine.count() >= columns.count()) {
                    Process proc;

                    proc.setPid(procLine.at(0).toLong());
                    proc.setRss(procLine.at(1).toLong() << 10);
                    proc.setPmem(procLine.at(2).toDouble());
                    proc.setVsize(procLine.at(3).toLong() << 10);
                    proc.setUname(procLine.at(4));
                    proc.setPcpu(procLine.at(5).toDouble());
                    proc.setStartTime(procLine.at(6));
                    proc.setState(procLine.at(7));
                    proc.setGroup(procLine.at(8));
                    proc.setNice(procLine.at(9).toInt());
                    proc.setCpuTime(procLine.at(10));
                    proc.setSession(procLine.at(11));
                    proc.setSeat(procLine.at(12));
                    proc.setCmd(procLine.at(13));

                    processList << proc;
                }
            }
        }

    } catch (QString &ex) {
        qCritical() << ex;
    }
}

QList<Process> ProcessInfo::getProcessList() const
{
    return processList;
}
