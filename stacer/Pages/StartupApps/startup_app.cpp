#include "startup_app.h"
#include "ui_startup_app.h"
#include "utilities.h"

StartupApp::~StartupApp()
{
    delete ui;
}

StartupApp::StartupApp(const QString &appName, bool enabled, const QString &filePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartupApp),
    startupAppEdit(new StartupAppEdit(this)),
    appName(appName),
    enabled(enabled),
    filePath(filePath)
{
    ui->setupUi(this);

    ui->appName->setText(appName);
    ui->startupCheck->setChecked(enabled);

    Utilities::addDropShadow(this, 50);

    connect(startupAppEdit, &StartupAppEdit::closeWindow, (StartupAppsPage*)parent, &StartupAppsPage::loadApps);
}

void StartupApp::on_startupCheck_clicked(bool status)
{
    QStringList lines = FileUtil::readListFromFile(filePath);

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

    FileUtil::writeFile(filePath, lines.join('\n').append('\n'));
}

void StartupApp::on_deleteAppBtn_clicked()
{
    if(QFile::remove(filePath))
        emit deleteApp();
}

void StartupApp::on_editAppBtn_clicked()
{
    StartupAppEdit::selectedFilePath = filePath;
    startupAppEdit->show();
}

QString StartupApp::getAppName() const
{
    return appName;
}

void StartupApp::setAppName(const QString &value)
{
    appName = value;
}

bool StartupApp::getEnabled() const
{
    return enabled;
}

void StartupApp::setEnabled(bool value)
{
    enabled = value;
}

QString StartupApp::getFilePath() const
{
    return filePath;
}

void StartupApp::setFilePath(const QString &value)
{
    filePath = value;
}

