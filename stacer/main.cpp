
#include <QApplication>
#include <QSplashScreen>
#include <QDebug>
#include <QFontDatabase>

#include "app.h"

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message)
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

        static QString logPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

        QFile file(logPath + "/stacer.log");

        QIODevice::OpenMode openMode = file.size() > (1L << 20) ? QIODevice::Truncate : QIODevice::Append;

        if (file.open(QIODevice::WriteOnly | openMode)) {
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
    qApp->setApplicationVersion("1.1.0");
    qApp->setWindowIcon(QIcon(":/static/logo.png"));

    {
        QCommandLineParser parser;
        parser.addVersionOption();
        parser.addHelpOption();
        parser.process(app);
    }

    QFontDatabase::addApplicationFont(":/static/font/Ubuntu-R.ttf");

    QPixmap pixSplash(":/static/splashscreen.png");

    QSplashScreen *splash = new QSplashScreen(pixSplash);

    splash->show();

    app.processEvents();

    App w;

    QLatin1String hideOption("--hide");

    if (argc < 2 || QString(argv[1]) != hideOption) {
        w.show();
    }

    splash->finish(&w);

    delete splash;

    return app.exec();
}
