#include "startup_apps_page.h"
#include "ui_startup_apps_page.h"
#include "utilities.h"

StartupAppsPage::~StartupAppsPage()
{
    delete ui;
}

StartupAppsPage::StartupAppsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartupAppsPage),
    startupAppEdit(new StartupAppEdit(this))
{
    ui->setupUi(this);

    init();
}

void StartupAppsPage::init()
{
    loadApps();

    connect(startupAppEdit, &StartupAppEdit::closeWindow, this, &StartupAppsPage::loadApps);

    Utilities::addDropShadow(ui->addStartupAppBtn, 60);
}

void StartupAppsPage::loadApps()
{
    // clear layout
    ui->startupListWidget->clear();

    static QString autostartPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart";

    QDir autostartFiles(autostartPath, "*.desktop");

    QChar sep('=');
    QRegExp nameFilter("(\\W+|^)Name=");
    QRegExp enabledFilter("(\\W+|^)X-GNOME-Autostart-enabled=");
    QLatin1String enabledStr("true");
    for (const QFileInfo &f : autostartFiles.entryInfoList())
    {
        QStringList lines = FileUtil::readListFromFile(f.absoluteFilePath());

        QStringList d_name = lines.filter(nameFilter); // get name

        if(! d_name.isEmpty()) // has a name
        {
            QString appName = d_name.first().split(sep).last().trimmed();

            QStringList d_autostart = lines.filter(enabledFilter);

            bool enabled = false;
            if(! d_autostart.isEmpty())
            {
                // X-GNOME-Autostart-enabled=[true|false]
                QString status = d_autostart.first()
                        .split(sep).last().toLower().trimmed();

                enabled = ! status.compare(enabledStr);
            }

            QListWidgetItem *item = new QListWidgetItem(ui->startupListWidget);
            // new app
            StartupApp *app = new StartupApp(appName, enabled, f.absoluteFilePath(), this);

            // delete button click
            connect(app, &StartupApp::deleteApp, this, &StartupAppsPage::loadApps);

            item->setSizeHint(app->sizeHint());

            ui->startupListWidget->setItemWidget(item, app);
        }
    }

    setAppCount();
}

void StartupAppsPage::setAppCount()
{
    int count = ui->startupListWidget->count();

    ui->startupAppsTitleLbl->setText(
        tr("System Startup Applications (%1)")
        .arg(QString::number(count)));

    ui->notFoundWidget->setVisible(! count);
    ui->startupListWidget->setVisible(count);
}

void StartupAppsPage::on_addStartupAppBtn_clicked()
{
    StartupAppEdit::selectedFilePath = "";
    startupAppEdit->show();
}
