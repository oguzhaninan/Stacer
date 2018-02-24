#include "app.h"
#include "ui_app.h"
#include "utilities.h"

App::~App()
{
    delete ui;
}

App::App(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::App),
    slidingStacked(new SlidingStackedWidget(this)),
    dashboardPage(new DashboardPage(slidingStacked)),
    startupAppsPage(new StartupAppsPage(slidingStacked)),
    systemCleanerPage(new SystemCleanerPage(slidingStacked)),
    servicesPage(new ServicesPage(slidingStacked)),
    processPage(new ProcessesPage(slidingStacked)),
    uninstallerPage(new UninstallerPage(slidingStacked)),
    resourcesPage(new ResourcesPage(slidingStacked)),
    gnomeSettingsPage(new GnomeSettingsPage(slidingStacked)),
    settingsPage(new SettingsPage(slidingStacked)),
    feedback(new Feedback(this)),
    trayIcon(new QSystemTrayIcon(QIcon(":/static/logo.png"), this))
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
        btn->setIconSize(QSize(28, 28));

    ui->pageContentLayout->addWidget(slidingStacked);

    // add pages
    slidingStacked->addWidget(dashboardPage);
    slidingStacked->addWidget(startupAppsPage);
    slidingStacked->addWidget(systemCleanerPage);
    slidingStacked->addWidget(servicesPage);
    slidingStacked->addWidget(processPage);
    slidingStacked->addWidget(uninstallerPage);
    slidingStacked->addWidget(resourcesPage);
    slidingStacked->addWidget(gnomeSettingsPage);
    slidingStacked->addWidget(settingsPage);

    // APT SOURCE MANAGER
    if (ToolManager::ins()->checkSourceRepository()) {
        aptSourceManagerPage = new APTSourceManagerPage(this);
        slidingStacked->addWidget(aptSourceManagerPage);
    } else {
        ui->aptSourceManagerBtn->hide();
    }

    AppManager::ins()->updateStylesheet();

    Utilities::addDropShadow(ui->sidebar, 60);

    connect(trayIcon, &QSystemTrayIcon::activated, this, &App::iconActivated);
    trayIcon->show();
}

void App::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        qDebug() << "trigger";
    case QSystemTrayIcon::DoubleClick:
        qDebug() << "double";
        break;
    case QSystemTrayIcon::MiddleClick:
        qDebug() << "middle";
        break;
    default:
        ;
    }
}

void App::pageClick(QWidget *w, QString title)
{
    ui->pageTitle->setText(title);
    slidingStacked->slideInIdx(slidingStacked->indexOf(w));
}

void App::on_dashBtn_clicked()
{
    pageClick(dashboardPage, tr("Dashboard"));
}

void App::on_systemCleanerBtn_clicked()
{
    pageClick(systemCleanerPage, tr("System Cleaner"));
}

void App::on_startupAppsBtn_clicked()
{
    pageClick(startupAppsPage, tr("System Startup Apps"));
}

void App::on_servicesBtn_clicked()
{
    pageClick(servicesPage, tr("System Services"));
}

void App::on_uninstallerBtn_clicked()
{
    pageClick(uninstallerPage, tr("Uninstaller"));
}

void App::on_resourcesBtn_clicked()
{
    pageClick(resourcesPage, tr("Resources"));
}

void App::on_processesBtn_clicked()
{
    pageClick(processPage, tr("Processes"));
}

void App::on_settingsBtn_clicked()
{
    pageClick(settingsPage, tr("Settings"));
}

void App::on_aptSourceManagerBtn_clicked()
{
    pageClick(aptSourceManagerPage, tr("APT Source Manager"));
}

void App::on_gnomeSettingsBtn_clicked()
{
    pageClick(gnomeSettingsPage, tr("Gnome Settings"));
}

void App::on_feedbackBtn_clicked()
{
    feedback->show();
}
