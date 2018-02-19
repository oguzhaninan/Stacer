#ifndef GNOME_SETTINGS_PAGE_H
#define GNOME_SETTINGS_PAGE_H

#include <QWidget>
#include "slidingstackedwidget.h"

#include "unity_settings.h"
#include "window_manager_settings.h"
#include "appearance_settings.h"

namespace Ui {
class GnomeSettingsPage;
}

class GnomeSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit GnomeSettingsPage(QWidget *parent = 0);
    ~GnomeSettingsPage();

private slots:

    void on_btnUnitySettings_clicked();

    void on_btnWindowManager_clicked();

    void on_btnAppearance_clicked();

private:
    void init();

private:
    Ui::GnomeSettingsPage *ui;

    SlidingStackedWidget *slidingStackedWidget;

    UnitySettings *unitySettings;
    WindowManagerSettings *windowManagerSettings;
    AppearanceSettings *appearanceSettings;
};

#endif // GNOME_SETTINGS_PAGE_H
