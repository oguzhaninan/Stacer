#include "app.h"
#include "ui_app.h"
#include "utilities.h"
#include <QStyle>
#include <QDebug>

App::~App()
{
    delete ui;
}

App::App(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::App),
    mSlidingStacked(new SlidingStackedWidget(this)),
    mTrayIcon(AppManager::ins()->getTrayIcon()),
    mTrayMenu(new QMenu(this))
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

    dashboardPage = new DashboardPage(mSlidingStacked);
    startupAppsPage = new StartupAppsPage(mSlidingStacked);
    searchPage = new SearchPage(mSlidingStacked);
    systemCleanerPage = new SystemCleanerPage(mSlidingStacked);
    servicesPage = new ServicesPage(mSlidingStacked);
    processPage = new ProcessesPage(mSlidingStacked);
    helpersPage = new HelpersPage(mSlidingStacked);
    uninstallerPage = new UninstallerPage(mSlidingStacked);
    resourcesPage = new ResourcesPage(mSlidingStacked);
    settingsPage = new SettingsPage(mSlidingStacked);

    ui->pageContentLayout->addWidget(mSlidingStacked);

    mListPages = {
        dashboardPage, startupAppsPage, systemCleanerPage, searchPage, servicesPage,
        processPage, uninstallerPage, resourcesPage, helpersPage, settingsPage
    };

    mListSidebarButtons = {
        ui->btnDash, ui->btnStartupApps, ui->btnSystemCleaner, ui->btnSearch, ui->btnServices,
        ui->btnProcesses, ui->btnHelpers, ui->btnUninstaller, ui->btnResources, ui->btnSettings
    };

    // APT SOURCE MANAGER
    if (ToolManager::ins()->checkSourceRepository()) {
        aptSourceManagerPage = new APTSourceManagerPage(mSlidingStacked);
        mListPages.insert(7, aptSourceManagerPage);
        mListSidebarButtons.insert(7, ui->btnAptSourceManager);
    } else {
        ui->btnAptSourceManager->hide();
    }

    // GNOME SETTINGS
    bool checkDesktopSession = QString(qgetenv("DESKTOP_SESSION")).contains(QRegExp("ubuntu", Qt::CaseInsensitive));
    bool checkDistribution = SystemInfo().getDistribution().contains(QRegExp("ubuntu", Qt::CaseInsensitive));;

    if (checkDesktopSession || checkDistribution) {
        gnomeSettingsPage = new GnomeSettingsPage(mSlidingStacked);
        mListPages.insert(8, gnomeSettingsPage);
        mListSidebarButtons.insert(8, ui->btnGnomeSettings);
    } else {
        ui->btnGnomeSettings->hide();
    }

    // add pages
    for (QWidget *page: mListPages) {
        mSlidingStacked->addWidget(page);
    }

    AppManager::ins()->updateStylesheet();

    Utilities::addDropShadow(ui->sidebar, 60);

    // set start page
    clickSidebarButton(SettingManager::ins()->getStartPage());

    createTrayActions();

    mTrayIcon->show();

    createQuitMessageBox();
}

void App::createQuitMessageBox()
{
    mBtnQuit = new QPushButton(tr("Quit"), this);
    mBtnQuit->setAccessibleName("danger");
    mBtnContinue = new QPushButton(tr("Continue"), this);
    mBtnContinue->setAccessibleName("primary");
    mQuitMsgBox = new QMessageBox(this);
    QCheckBox *check = new QCheckBox("Don't ask again.");
    check->setAccessibleName("circle");
    mQuitMsgBox->setWindowTitle(tr("Quit"));
    mQuitMsgBox->setText(tr("Will the program continue to work in the system tray?"));
    mQuitMsgBox->addButton(mBtnQuit, QMessageBox::YesRole);
    mQuitMsgBox->addButton(mBtnContinue, QMessageBox::NoRole);
    mQuitMsgBox->setCheckBox(check);

    connect(check, &QCheckBox::toggled, [this](bool checked) {
        SettingManager::ins()->setAppQuitDialogDontAsk(checked);
    });
}

void App::closeEvent(QCloseEvent *event)
{
    if (SettingManager::ins()->getAppQuitDialogDontAsk()) {
        if (SettingManager::ins()->getAppQuitDialogChoice() == "close") {
            event->accept();
        } else {
            event->ignore();
            hide();
        }
    } else {
        mQuitMsgBox->exec();
        if (mQuitMsgBox->clickedButton() == mBtnContinue) {
            SettingManager::ins()->setAppQuitDialogChoice("hide");
            event->ignore();
            hide();
        } else if (mQuitMsgBox->clickedButton() == mBtnQuit) {
            SettingManager::ins()->setAppQuitDialogChoice("close");
            event->accept();
        } else {
            event->ignore();
        }
    }
}

void App::createTrayActions()
{
    for (QPushButton *button: mListSidebarButtons) {
        QString toolTip = button->toolTip();
        QAction *action = new QAction(toolTip, this);
        connect(action, &QAction::triggered, [=] {
            clickSidebarButton(toolTip, true);
        });
        connect(mTrayIcon, &QSystemTrayIcon::activated, this, [=](QSystemTrayIcon::ActivationReason){
            setVisible(true);
            activateWindow();	
        });

        mTrayMenu->addAction(action);
    }

    mTrayMenu->addSeparator();

    QAction *quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, &QAction::triggered, [=] {qApp->quit();});
    mTrayMenu->addAction(quitAction);

    mTrayIcon->setContextMenu(mTrayMenu);
}

void App::clickSidebarButton(QString pageTitle, bool isShow)
{
    QWidget *selectedWidget = getPageByTitle(pageTitle);
    if (selectedWidget) {
        pageClick(selectedWidget, !isShow);
        checkSidebarButtonByTooltip(pageTitle);
    } else {
        pageClick(mListPages.first());
    }
    setVisible(isShow);
    if (isShow) activateWindow();
}

void App::checkSidebarButtonByTooltip(const QString &text)
{
    for (QPushButton *button: mListSidebarButtons) {
        if (button->toolTip() == text) {
            button->setChecked(true);
        }
    }
}

QWidget* App::getPageByTitle(const QString &title)
{
    for (QWidget *page: mListPages) {
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
            mSlidingStacked->slideInIdx(mSlidingStacked->indexOf(widget));
        } else {
            mSlidingStacked->setCurrentWidget(widget);
        }
    }
}

void App::on_btnDash_clicked()
{
    pageClick(dashboardPage);
}

void App::on_btnStartupApps_clicked()
{
    pageClick(startupAppsPage);
}

void App::on_btnSystemCleaner_clicked()
{
    pageClick(systemCleanerPage);
}

void App::on_btnSearch_clicked()
{
    pageClick(searchPage);
}

void App::on_btnServices_clicked()
{
    pageClick(servicesPage);
}

void App::on_btnUninstaller_clicked()
{
    pageClick(uninstallerPage);
}

void App::on_btnProcesses_clicked()
{
    pageClick(processPage);
}

void App::on_btnResources_clicked()
{
    pageClick(resourcesPage);
}

void App::on_btnHelpers_clicked()
{
    pageClick(helpersPage);
}

void App::on_btnAptSourceManager_clicked()
{
    pageClick(aptSourceManagerPage);
}

void App::on_btnGnomeSettings_clicked()
{
    pageClick(gnomeSettingsPage);
}

void App::on_btnSettings_clicked()
{
    pageClick(settingsPage);
}

void App::on_btnFeedback_clicked()
{
    if (feedback.isNull()) {
        feedback = QSharedPointer<Feedback>(new Feedback(this));
    }
    feedback->show();
}
