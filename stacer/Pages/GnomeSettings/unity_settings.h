#ifndef UNITY_SETTINGS_H
#define UNITY_SETTINGS_H

#include <QWidget>
#include "Tools/gnome_settings_tool.h"

namespace Ui {
class UnitySettings;
}

class UnitySettings : public QWidget
{
    Q_OBJECT

public:
    explicit UnitySettings(QWidget *parent = 0);
    ~UnitySettings();

public:
    void init();

private slots:
    void on_checkLauncherAutoHide_clicked(bool checked);

    void on_radioRevealLeft_clicked();
    void on_radioRevealTopLeft_clicked();

    void on_sliderRevealSensitivy_valueChanged(int value);

    void on_checkMinimizeApps_clicked(bool checked);

    void on_sliderLauncherOpacity_valueChanged(int value);

    void on_radioLauncherVisibleAllDesktop_clicked();
    void on_radioLauncherVisiblePrimaryDesktop_clicked();

    void on_radioLauncherPositionLeft_clicked();
    void on_radioLauncherPositionBottom_clicked();

    void on_spinIconSize_valueChanged(int value);

    void on_checkBackgroundBlur_clicked(bool checked);

    void on_checkSearchOnlineResource_clicked(bool checked);

    void on_checkMoreSuggestions_clicked(bool checked);

    void on_checkRecentlyUsed_clicked(bool checked);

    void on_checkSearchYourFiles_clicked(bool checked);

    void on_sliderPanelOpacity_valueChanged(int value);

    void on_checkDateTime_clicked(bool checked);

    void on_check24Hour_clicked(bool checked);

    void on_checkSeconds_clicked(bool checked);

    void on_checkDate_clicked(bool checked);

    void on_checkWeekday_clicked(bool checked);

    void on_checkCalendar_clicked(bool checked);

    void on_checkVolume_clicked(bool checked);

    void on_checkShowMyName_clicked(bool checked);

private:
    Ui::UnitySettings *ui;

    GnomeSettingsTool gsettings;
};

#endif // UNITY_SETTINGS_H
