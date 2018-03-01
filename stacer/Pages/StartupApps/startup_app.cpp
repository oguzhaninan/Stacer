#include "startup_app.h"
#include "ui_startup_app.h"
#include "utilities.h"

StartupApp::~StartupApp()
{
    delete ui;
}

StartupApp::StartupApp(const QString &lblStartupAppName, bool enabled, const QString &filePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartupApp),
    lblStartupAppName(lblStartupAppName),
    enabled(enabled),
    filePath(filePath)
{
    ui->setupUi(this);

    ui->lblStartupAppName->setText(lblStartupAppName);
    ui->checkStartup->setChecked(enabled);

    Utilities::addDropShadow(this, 50);
}

void StartupApp::on_checkStartup_clicked(bool status)
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

void StartupApp::on_btnDeleteStartupApp_clicked()
{
    if(QFile::remove(filePath))
        emit deleteAppS();
}

void StartupApp::on_btnEditStartupApp_clicked()
{
    emit editStartupAppS(filePath);
}

QString StartupApp::getAppName() const
{
    return lblStartupAppName;
}

void StartupApp::setAppName(const QString &value)
{
    lblStartupAppName = value;
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

