#include "command_util.h"

#include <QProcess>
#include <QTextStream>
#include <QStandardPaths>
#include <QDebug>

CommandUtil::CommandUtil()
{

}

QString CommandUtil::sudoExec(const QString &cmd, QStringList args)
{
    args.push_front(cmd);

    QString result("");

    try {
        result = CommandUtil::exec("pkexec", args);
    } catch (QString &ex) {
        qCritical() << ex;
    }

    return result;
}

QString CommandUtil::exec(const QString &cmd, QStringList args)
{
    QProcess* process = new QProcess;

    if(args.isEmpty())
        process->start(cmd);
    else
        process->start(cmd, args);

    process->waitForFinished();

    QTextStream stdOut(process->readAllStandardOutput());

    QString err = process->errorString();

    process->kill();
    process->close();

    if (process->error() != QProcess::UnknownError)
        throw err;

    return stdOut.readAll().trimmed();
}

bool CommandUtil::isExecutable(const QString &cmd)
{
    return !QStandardPaths::findExecutable(cmd).isEmpty();
}
