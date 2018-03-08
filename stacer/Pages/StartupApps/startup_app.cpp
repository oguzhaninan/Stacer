#include "startup_app.h"
#include "ui_startup_app.h"
#include "utilities.h"

StartupApp::~StartupApp()
{
    delete ui;
}

StartupApp::StartupApp(const QString &startupAppName, bool enabled, const QString &filePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartupApp),
    mStartupAppName(startupAppName),
    mEnabled(enabled),
    mFilePath(filePath)
{
    ui->setupUi(this);

    ui->lblStartupAppName->setText(startupAppName);
    ui->checkStartup->setChecked(enabled);

    Utilities::addDropShadow(this, 50);
}

void StartupApp::on_checkStartup_clicked(bool status)
{
    QStringList lines = FileUtil::readListFromFile(mFilePath);

    // Hidden=[true|false]
    int pos = lines.indexOf(HIDDEN_REG);

    QString _status = status ? "true" : "false";

    if (pos != -1) {
        _status = status ? "false" : "true";
        lines.replace(pos, QString("Hidden=%1").arg(_status));
    } else {
        // X-GNOME-Autostart-enabled=[true|false]
        pos = lines.indexOf(GNOME_ENABLED_REG);
        if (pos != -1) {
            lines.replace(pos, QString("X-GNOME-Autostart-enabled=%1").arg(_status));
        }
    }

    if (pos == -1) {
        _status = status ? "false" : "true";
        lines.append(QString("Hidden=%1").arg(_status));
    }

    FileUtil::writeFile(mFilePath, lines.join('\n').append('\n'));
}

void StartupApp::on_btnDeleteStartupApp_clicked()
{
    if (QFile::remove(mFilePath)) {
        emit deleteAppS();
    }
}

void StartupApp::on_btnEditStartupApp_clicked()
{
    emit editStartupAppS(mFilePath);
}

QString StartupApp::getAppName() const
{
    return mStartupAppName;
}

void StartupApp::setAppName(const QString &value)
{
    mStartupAppName = value;
}

bool StartupApp::getEnabled() const
{
    return mEnabled;
}

void StartupApp::setEnabled(bool value)
{
    mEnabled = value;
}

QString StartupApp::getFilePath() const
{
    return mFilePath;
}

void StartupApp::setFilePath(const QString &value)
{
    mFilePath = value;
}

