#include <QApplication>
#include <QSplashScreen>
#include <QDebug>
#include "app.h"
#include "utilities.h"
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
    qApp->setApplicationVersion("1.0.9");
    qApp->setWindowIcon(QIcon(":/static/logo.png"));


    QGraphicsDropShadowEffect *splashShadowEffect = new QGraphicsDropShadowEffect;
    splashShadowEffect->setBlurRadius(30);
    splashShadowEffect->setColor(QColor(0, 0, 0, 210));
    splashShadowEffect->setOffset(0);


    QPixmap pixmap(":/static/splashscreen.png");
    QSplashScreen splash(pixmap);
    splash.setGraphicsEffect(splashShadowEffect);


    splash.show();

    app.processEvents();

    QThread::sleep(10);
//    qInstallMessageHandler(messageHandler);

    App w;

    w.show();

    splash.finish(&w);

    return app.exec();
}
