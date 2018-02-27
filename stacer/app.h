#ifndef APP_H
#define APP_H

#include <QMainWindow>

#include "slidingstackedwidget.h"
#include "Managers/app_manager.h"

// Pages
#include "Pages/Dashboard/dashboard_page.h"
#include "Pages/StartupApps/startup_apps_page.h"
#include "Pages/SystemCleaner/system_cleaner_page.h"
#include "Pages/Services/services_page.h"
#include "Pages/Processes/processes_page.h"
#include "Pages/Uninstaller/uninstaller_page.h"
#include "Pages/Resources/resources_page.h"
#include "Pages/Settings/settings_page.h"
#include "Pages/AptSourceManager/apt_source_manager_page.h"
#include "Pages/GnomeSettings/gnome_settings_page.h"
#include "feedback.h"

namespace Ui {
    class App;
}

class App : public QMainWindow
{
    Q_OBJECT

public:
    explicit App(QWidget *parent = 0);
    ~App();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void init();
    void pageClick(QWidget *widget, bool slide = true);
    void clickSidebarButton(QString pageTitle, bool isShow = false);

    void on_dashBtn_clicked();
    void on_systemCleanerBtn_clicked();
    void on_startupAppsBtn_clicked();
    void on_servicesBtn_clicked();
    void on_uninstallerBtn_clicked();
    void on_resourcesBtn_clicked();
    void on_processesBtn_clicked();
    void on_settingsBtn_clicked();
    void on_gnomeSettingsBtn_clicked();
    void on_aptSourceManagerBtn_clicked();

    void on_feedbackBtn_clicked();

private:
    QWidget *getPageByTitle(const QString &title);
    void checkSidebarButtonByTooltip(const QString &text);
    void createTrayActions();

private:
    Ui::App *ui;

    // Pages
    QList<QWidget*> listPages;
    QList<QPushButton*> listSidebarButtons;

    SlidingStackedWidget *slidingStacked;

    DashboardPage *dashboardPage;
    StartupAppsPage *startupAppsPage;
    SystemCleanerPage *systemCleanerPage;
    ServicesPage *servicesPage;
    ProcessesPage *processPage;
    UninstallerPage *uninstallerPage;
    ResourcesPage *resourcesPage;
    APTSourceManagerPage *aptSourceManagerPage;
    GnomeSettingsPage *gnomeSettingsPage;
    SettingsPage *settingsPage;

    QSharedPointer<Feedback> feedback;

    QSystemTrayIcon *trayIcon;

    QMenu *trayMenu;
};

#endif // APP_H
