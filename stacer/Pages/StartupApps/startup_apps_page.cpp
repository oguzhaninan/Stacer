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

bool StartupAppsPage::checkIfDisabled(const QString& as_path)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QByteArrayView disabled_str("X-GNOME-Autostart-enabled=false");
#else
    const QString disabled_str("X-GNOME-Autostart-enabled=false");
#endif

    QFile autostart_file(as_path);

    autostart_file.open(QIODevice::ReadOnly | QIODevice::Text);

    return autostart_file.readAll().indexOf(disabled_str, 0) != -1;
}

void StartupAppsPage::init()
{
    mAutostartPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).append("/autostart");
    QFileInfo asfi(mAutostartPath);
    bool startups_disabled = false;

    /* original behavior, autostart is a dir and not...
     * * a pre-existing file as is case on my machine.
     */
    if (asfi.isDir()) {
        mAutostartPath.append("/");
    }
    else {
    /* altered behavior for if a file is at this location instead
     * * check for disabled string
     * * * if found, don't add watcher
     */
        startups_disabled = checkIfDisabled(mAutostartPath);
    }

    if (!startups_disabled) {
        if (! QDir(mAutostartPath).exists()) {
            QDir().mkdir(mAutostartPath);
        }

        mFileSystemWatcher.addPath(mAutostartPath);

        loadApps();

        connect(&mFileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, &StartupAppsPage::loadApps);
    }
    else {
        ui->lblNotFound->setText(tr("Startup Apps are disabled."));
        ui->btnAddStartupApp->setEnabled(false);
    }

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

            auto *item = new QListWidgetItem(ui->listWidgetStartup);

            // new app
            auto *app = new StartupApp(appName, enabled, f.absoluteFilePath(), this);

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
        tr("Startup Applications (%1)")
        .arg(QString::number(count)));

    ui->notFoundWidget->setVisible(! count);
    ui->listWidgetStartup->setVisible(count);
}

void StartupAppsPage::openStartupAppEdit(const QString& filePath)
{
    StartupAppEdit::selectedFilePath = filePath;
    if (mStartupAppEdit.isNull()) {
        mStartupAppEdit = QSharedPointer<StartupAppEdit>(new StartupAppEdit(this));
        connect(mStartupAppEdit.data(), &StartupAppEdit::startupAppAdded, this, &StartupAppsPage::loadApps);
    }
    mStartupAppEdit->show();
}
