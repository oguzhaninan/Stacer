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
    mFileSystemWatcher(this)
{
    ui->setupUi(this);

    init();
}

void StartupAppsPage::init()
{
    this->mAutostartPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).append("/autostart");
    if (! QDir(this->mAutostartPath).exists()) {
        QDir().mkdir(this->mAutostartPath);
    }

    mFileSystemWatcher.addPath(this->mAutostartPath);

    loadApps();

    connect(&mFileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, &StartupAppsPage::loadApps);
    connect(ui->btnAddStartupApp, SIGNAL(clicked()), this, SLOT(openStartupAppEdit()));

    Utilities::addDropShadow(ui->btnAddStartupApp, 60);
}

void StartupAppsPage::loadApps()
{
    // clear
    ui->listWidgetStartup->clear();

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

            QListWidgetItem *item = new QListWidgetItem(ui->listWidgetStartup);

            // new app
            StartupApp *app = new StartupApp(appName, enabled, f.absoluteFilePath(), this);

            connect(app, &StartupApp::deleteAppS, this, &StartupAppsPage::loadApps);
            connect(app, &StartupApp::editStartupAppS, this, &StartupAppsPage::openStartupAppEdit);

            item->setSizeHint(app->sizeHint());

            ui->listWidgetStartup->setItemWidget(item, app);
        }
    }

    setAppCount();
}

void StartupAppsPage::setAppCount()
{
    int count = ui->listWidgetStartup->count();

    ui->lblStartupAppsTitle->setText(
        tr("System Startup Applications (%1)")
        .arg(QString::number(count)));

    ui->notFoundWidget->setVisible(! count);
    ui->listWidgetStartup->setVisible(count);
}

void StartupAppsPage::openStartupAppEdit(const QString filePath)
{
    StartupAppEdit::selectedFilePath = filePath;
    if (startupAppEdit.isNull()) {
        startupAppEdit = QSharedPointer<StartupAppEdit>(new StartupAppEdit(this));
        connect(startupAppEdit.data(), &StartupAppEdit::startupAppAdded, this, &StartupAppsPage::loadApps);
    }
    startupAppEdit->show();
}
