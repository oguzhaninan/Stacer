#include <QApplication>
#include <QSplashScreen>
#include <QDebug>
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
    qApp->setApplicationVersion("1.0.9");
    qApp->setWindowIcon(QIcon(":/static/logo.png"));

    QPixmap pixmap(":/static/splashscreen.png");

    QGraphicsDropShadowEffect *splashShadowEffect = new QGraphicsDropShadowEffect;
    splashShadowEffect->setBlurRadius(30);
    splashShadowEffect->setColor(QColor(0, 0, 0, 210));
    splashShadowEffect->setOffset(0);

    QSplashScreen splash(pixmap);

    splash.setGraphicsEffect(splashShadowEffect);

    splash.show();

    app.processEvents();

//    QThread::sleep(1);

//    qInstallMessageHandler(messageHandler);

    App w;

    QLatin1String hideOption("--hide");

    if (argc > 1) {
        QLatin1String option(argv[1]);
        // hide
        if (option != hideOption) {
            w.show();
        }
    } else {
        w.show();
    }

    splash.finish(&w);

    return app.exec();
}
