#include "startup_app.h"
#include "ui_startup_app.h"

StartupApp::~StartupApp()
{
    delete ui;
    delete startupAppEdit;
}

StartupApp::StartupApp(QString appName, bool enabled, QString filePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartupApp),
    startupAppEdit(new StartupAppEdit),
    appName(appName),
    enabled(enabled),
    filePath(filePath)
{
    ui->setupUi(this);

    ui->appName->setText(appName);
    ui->startupCheck->setChecked(enabled);

    ui->deleteAppBtn->setToolTip(tr("Delete"));
    ui->editAppBtn->setToolTip(tr("Edit"));

    connect(startupAppEdit, &StartupAppEdit::closeWindow, (StartupAppsPage*)parent, &StartupAppsPage::loadApps);
}

void StartupApp::on_startupCheck_clicked(bool status)
{
    QStringList lines = FileUtil::readListFromFile(filePath);

    int pos = lines.indexOf(QRegExp("X-GNOME-Autostart-enabled=.*"));

    if(pos != -1)
    {
        lines.replace(pos, QString("X-GNOME-Autostart-enabled=%1").arg(status ? "true" : "false"));

        FileUtil::writeFile(filePath, lines.join("\n"));
    }
}

void StartupApp::on_deleteAppBtn_clicked()
{
    if(QFile::remove(filePath))
        deleteApp();
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

