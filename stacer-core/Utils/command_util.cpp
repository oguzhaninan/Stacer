#include "command_util.h"

#include <QProcess>
#include <QTextStream>
#include <QStandardPaths>
#include <QDebug>

QString CommandUtil::sudoExec(const QString &cmd, QStringList args, QByteArray data)
{
    /*
     * QList::push_front, equivalent to QList::prepend, is
     * used for taking a QString and pushing it to the
     * beginning of a QList. QStringList inherits QList
     * by definition, so this call will take the value of
     * the QString cmd and insert it at the front of
     * the QStringList args.
     */
    args.push_front(cmd);

    // Used to hold function return value
    QString result("");

    try {
        // the 'pkexec' command in Linux will execute the command as the superuser
        result = CommandUtil::exec("pkexec", args, data);
    } catch (QString &ex) {
        qCritical() << ex;
    }

    return result;
}

QString CommandUtil::exec(const QString &cmd, QStringList args, QByteArray data)
{
    /*
     * A QProcess will start an external process and communicate with it.
    */
    QProcess* process = new QProcess;

    /*
     * The QProcess::start function will pass first argument to the command line
     * and the second argument as the commands options/arguments.
    */
    process->start(cmd, args);

    if (! data.isEmpty()) {
        process->write(data);
        process->waitForBytesWritten();
        process->closeWriteChannel();
    }

    // Will pause until this process is completed.
    process->waitForFinished();
    // Returns all available data from stdout of process.
    QTextStream stdOut(process->readAllStandardOutput());
    // Get available data from stderr of process.
    QString err = process->errorString();

    // Terminate the process and close it
    process->kill();
    process->close();

    // If there is a valid error, throw it.
    if (process->error() != QProcess::UnknownError)
        throw err;

    // returns output info from stdout.
    return stdOut.readAll().trimmed();
}

bool CommandUtil::isExecutable(const QString &cmd)
{
    return !QStandardPaths::findExecutable(cmd).isEmpty();
}
