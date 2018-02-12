#include "process_info.h"

#include <QDebug>

void ProcessInfo::updateProcesses()
{
    processList.clear();

    try {

        QStringList columns = { "pid", "rss", "pmem", "vsize", "uname:50", "pcpu", "start_time",
                                "state", "group", "nice", "cputime", "session", "cmd"};

        QStringList lines = CommandUtil::exec("ps", {"ax", "-weo", columns.join(","), "--no-headings"})
                .trimmed()
                .split(QChar('\n'));

        if (! lines.isEmpty()) {
            QRegExp sep("\\s+");
            for (const QString &line : lines) {
                QStringList procLine = line.trimmed().split(sep);

                if (procLine.count() >= columns.count()) {
                    Process proc;

                    proc.setPid(procLine.takeFirst().toLong());
                    proc.setRss(procLine.takeFirst().toLong() << 10);
                    proc.setPmem(procLine.takeFirst().toDouble());
                    proc.setVsize(procLine.takeFirst().toLong() << 10);
                    proc.setUname(procLine.takeFirst());
                    proc.setPcpu(procLine.takeFirst().toDouble());
                    proc.setStartTime(procLine.takeFirst());
                    proc.setState(procLine.takeFirst());
                    proc.setGroup(procLine.takeFirst());
                    proc.setNice(procLine.takeFirst().toInt());
                    proc.setCpuTime(procLine.takeFirst());
                    proc.setSession(procLine.takeFirst());
                    proc.setCmd(procLine.join(" "));

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
