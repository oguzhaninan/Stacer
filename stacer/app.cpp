#include "app.h"
#include "ui_app.h"

#include "Managers/app_manager.h"

App::~App()
{
    delete ui;
}

App::App(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::App),
    dashboardPage(new DashboardPage(this)),
    startupAppsPage(new StartupAppsPage(this)),
    systemCleanerPage(new SystemCleanerPage(this)),
    servicesPage(new ServicesPage(this)),
    processPage(new ProcessesPage(this)),
    uninstallerPage(new UninstallerPage(this)),
    resourcesPage(new ResourcesPage(this)),
    settingsPage(new SettingsPage(this))
{
    ui->setupUi(this);

    init();
}

void App::init()
{
    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            size(),
            qApp->desktop()->availableGeometry())
    );

    // form settings
    ui->horizontalLayout->setContentsMargins(0,0,0,0);
    ui->horizontalLayout->setSpacing(0);

    // icon sizes of the buttons on the sidebar 30x30
    for (QPushButton *btn : ui->sidebar->findChildren<QPushButton*>())
        btn->setIconSize(QSize(26, 26));

    // add pages
    ui->pageStacked->addWidget(dashboardPage);
    ui->pageStacked->addWidget(startupAppsPage);
    ui->pageStacked->addWidget(systemCleanerPage);
    ui->pageStacked->addWidget(servicesPage);
    ui->pageStacked->addWidget(processPage);
    ui->pageStacked->addWidget(uninstallerPage);
    ui->pageStacked->addWidget(resourcesPage);
    ui->pageStacked->addWidget(settingsPage);

    on_dashBtn_clicked();
}

void App::pageClick(QPushButton *btn, QWidget *w, QString title)
{
    // all button checked false
    for (QPushButton *b : ui->sidebar->findChildren<QPushButton*>())
        b->setChecked(false);
    btn->setChecked(true); // clicked button set active style
    AppManager::ins()->updateStylesheet(); // update style

    ui->pageTitle->setText(title);
    ui->pageStacked->setCurrentWidget(w);
}

void App::on_dashBtn_clicked()
{
    pageClick(ui->dashBtn, dashboardPage, tr("Dashboard"));
}

void App::on_systemCleanerBtn_clicked()
{
    pageClick(ui->systemCleanerBtn, systemCleanerPage, tr("System Cleaner"));
}

void App::on_startupAppsBtn_clicked()
{
    pageClick(ui->startupAppsBtn, startupAppsPage, tr("System Startup Apps"));
}

void App::on_servicesBtn_clicked()
{
    pageClick(ui->servicesBtn, servicesPage, tr("System Services"));
}

void App::on_uninstallerBtn_clicked()
{
    pageClick(ui->uninstallerBtn, uninstallerPage, tr("Uninstaller"));
}

void App::on_resourcesBtn_clicked()
{
    pageClick(ui->resourcesBtn, resourcesPage, tr("Resources"));
}

void App::on_processesBtn_clicked()
{
    pageClick(ui->processesBtn, processPage, tr("Processes"));
}

void App::on_settingsBtn_clicked()
{
    pageClick(ui->settingsBtn, settingsPage, tr("Settings"));
}
