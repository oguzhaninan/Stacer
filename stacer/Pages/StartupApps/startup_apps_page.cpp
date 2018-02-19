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
    startupAppEdit(new StartupAppEdit(this)),
    mFileSystemWatcher(this)
{
    ui->setupUi(this);

    init();
}

void StartupAppsPage::init()
{
    this->mAutostartPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart";

    mFileSystemWatcher.addPath(this->mAutostartPath);

    loadApps();

    connect(startupAppEdit, &StartupAppEdit::closeWindow, this, &StartupAppsPage::loadApps);
    connect(&mFileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, &StartupAppsPage::loadApps);

    Utilities::addDropShadow(ui->addStartupAppBtn, 60);
}

void StartupAppsPage::loadApps()
{
    // clear layout
    ui->startupListWidget->clear();

    QDir autostartFiles(mAutostartPath, "*.desktop");

    QLatin1String enabledStr("true");
    for (const QFileInfo &f : autostartFiles.entryInfoList())
    {
        QStringList lines = FileUtil::readListFromFile(f.absoluteFilePath());

        QString appName = Utilities::getDesktopValue(NAME_REG, lines); // get name

        if(! appName.isEmpty()) // has a name
        {
            bool enabled = false;

            // Hidden=[true|false]
            QString hidden = Utilities::getDesktopValue(HIDDEN_REG, lines).toLower();

            // X-GNOME-Autostart-enabled=[true|false]
            QString gnomeEnabled = Utilities::getDesktopValue(GNOME_ENABLED_REG, lines).toLower();

            if (! hidden.isEmpty()) {
                enabled = (hidden != enabledStr);
            } else {
                enabled = (gnomeEnabled == enabledStr);
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
