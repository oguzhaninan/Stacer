#include <QApplication>
#include <QDebug>
#include "app.h"

void messageHandler(QtMsgType type,
                    const QMessageLogContext &context,
                    const QString &message)
{
    Q_UNUSED(context)

    QString level;

    switch (type) {
    case QtDebugMsg:
        level = "DEBUG"; break;
    case QtInfoMsg:
        level = "INFO"; break;
    case QtWarningMsg:
        level = "WARNING"; break;
    case QtCriticalMsg:
        level = "CRITICAL"; break;
    case QtFatalMsg:
        level = "FATAL"; break;
    default:
        level = "UNDEFIEND"; break;
    }

    if (type != QtWarningMsg) {

        QString text = QString("[%1] [%2] %3")
                                .arg(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"))
                                .arg(level)
                                .arg(message);

        static QString logPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/log";

        QDir().mkdir(logPath);

        QFile file(logPath + "/stacer.log");

        QIODevice::OpenMode openMode;

        if (file.size() > (1L << 20))
            openMode = QIODevice::WriteOnly | QIODevice::Truncate;
        else
            openMode = QIODevice::WriteOnly | QIODevice::Append;

        if (file.open(openMode)) {
            QTextStream stream(&file);
            stream << text << endl;

            file.close();
        }
    }
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qApp->setApplicationName("stacer");
    qApp->setApplicationDisplayName("Stacer");
    qApp->setApplicationVersion("1.0.8");
    qApp->setWindowIcon(QIcon(":/static/logo.png"));

    qInstallMessageHandler(messageHandler);

    App w;

    w.show();

    return app.exec();
}
