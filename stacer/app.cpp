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
    trayIcon(AppManager::ins()->getTrayIcon()),
    trayMenu(new QMenu(this))
{
    ui->setupUi(this);

    init();
}

void App::init()
{
    setGeometry(
        QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
            size(), qApp->desktop()->availableGeometry())
    );

    // form settings
    ui->horizontalLayout->setContentsMargins(0,0,0,0);
    ui->horizontalLayout->setSpacing(0);

    ui->pageContentLayout->addWidget(slidingStacked);

    listPages = {
        dashboardPage, startupAppsPage, systemCleanerPage, servicesPage, processPage,
        uninstallerPage, resourcesPage, gnomeSettingsPage, settingsPage
    };

    listSidebarButtons = {
        ui->dashBtn, ui->startupAppsBtn, ui->systemCleanerBtn, ui->servicesBtn, ui->processesBtn,
        ui->uninstallerBtn, ui->resourcesBtn, ui->gnomeSettingsBtn, ui->settingsBtn
    };

    // APT SOURCE MANAGER
    if (ToolManager::ins()->checkSourceRepository()) {
        aptSourceManagerPage = new APTSourceManagerPage(this);
        listPages.insert(7, aptSourceManagerPage);
        listSidebarButtons.insert(7, ui->aptSourceManagerBtn);
    } else {
        ui->aptSourceManagerBtn->hide();
    }

    // add pages
    for (QWidget *page: listPages) {
        slidingStacked->addWidget(page);
    }

    AppManager::ins()->updateStylesheet();

    Utilities::addDropShadow(ui->sidebar, 60);

    // set home page
    QString selectedHomePage = AppManager::ins()->getHomePage();
    clickSidebarButton(selectedHomePage);

    createTrayActions();

    trayIcon->show();
}

void App::closeEvent(QCloseEvent *event)
{
    event->accept();
//    event->ignore();
//    hide();
}

void App::createTrayActions()
{
    for (QPushButton *button: listSidebarButtons) {
        QString toolTip = button->toolTip();
        QAction *action = new QAction(toolTip, this);
        connect(action, &QAction::triggered, [=] {
            clickSidebarButton(toolTip, true);
        });
        trayMenu->addAction(action);
    }

    trayMenu->addSeparator();

    QAction *quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, &QAction::triggered, [=] {qApp->quit();});
    trayMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayMenu);
}

void App::clickSidebarButton(QString pageTitle, bool isShow)
{
    QWidget *selectedWidget = getPageByTitle(pageTitle);
    if (selectedWidget) {
        pageClick(selectedWidget, !isShow);
        checkSidebarButtonByTooltip(pageTitle);
    } else {
        pageClick(listPages.first());
    }
    if (isShow) {
        show();
    }
}

void App::checkSidebarButtonByTooltip(const QString &text)
{
    for (QPushButton *button: listSidebarButtons) {
        if (button->toolTip() == text) {
            button->setChecked(true);
        }
    }
}

QWidget* App::getPageByTitle(const QString &title)
{
    for (QWidget *page: listPages) {
        if (page->windowTitle() == title) {
            return page;
        }
    }
    return nullptr;
}

void App::pageClick(QWidget *widget, bool slide)
{
    if (widget) {
        ui->pageTitle->setText(widget->windowTitle());
        if (slide) {
            slidingStacked->slideInIdx(slidingStacked->indexOf(widget));
        } else {
            slidingStacked->setCurrentWidget(widget);
        }
    }
}

void App::on_dashBtn_clicked()
{
    pageClick(dashboardPage);
}

void App::on_startupAppsBtn_clicked()
{
    pageClick(startupAppsPage);
}

void App::on_systemCleanerBtn_clicked()
{
    pageClick(systemCleanerPage);
}

void App::on_servicesBtn_clicked()
{
    pageClick(servicesPage);
}

void App::on_uninstallerBtn_clicked()
{
    pageClick(uninstallerPage);
}

void App::on_processesBtn_clicked()
{
    pageClick(processPage);
}

void App::on_resourcesBtn_clicked()
{
    pageClick(resourcesPage);
}

void App::on_aptSourceManagerBtn_clicked()
{
    pageClick(aptSourceManagerPage);
}

void App::on_gnomeSettingsBtn_clicked()
{
    pageClick(gnomeSettingsPage);
}

void App::on_settingsBtn_clicked()
{
    pageClick(settingsPage);
}

void App::on_feedbackBtn_clicked()
{
    if (feedback.isNull()) {
        feedback = QSharedPointer<Feedback>(new Feedback(this));
    }
    feedback->show();
}
