#ifndef APP_H
#define APP_H

#include <QMainWindow>

#include "sliding_stacked_widget.h"
#include "Managers/app_manager.h"
#include "Managers/setting_manager.h"

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

    void on_btnDash_clicked();
    void on_btnSystemCleaner_clicked();
    void on_btnStartupApps_clicked();
    void on_btnServices_clicked();
    void on_btnUninstaller_clicked();
    void on_btnResources_clicked();
    void on_btnProcesses_clicked();
    void on_btnSettings_clicked();
    void on_btnGnomeSettings_clicked();
    void on_btnAptSourceManager_clicked();

    void on_btnFeedback_clicked();

private:
    QWidget *getPageByTitle(const QString &title);
    void checkSidebarButtonByTooltip(const QString &text);
    void createTrayActions();
    void createQuitMessageBox();

private:
    Ui::App *ui;

    // Pages
    QList<QWidget*> mListPages;
    QList<QPushButton*> mListSidebarButtons;

    SlidingStackedWidget *mSlidingStacked;

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

    QSystemTrayIcon *mTrayIcon;

    QMenu *mTrayMenu;

    QPushButton *mBtnQuit, *mBtnContinue;
    QMessageBox *mQuitMsgBox;
};

#endif // APP_H
