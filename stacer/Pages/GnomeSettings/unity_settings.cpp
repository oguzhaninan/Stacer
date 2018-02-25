#include "unity_settings.h"
#include "ui_unity_settings.h"
#include <QDebug>

UnitySettings::~UnitySettings()
{
    delete ui;
}

UnitySettings::UnitySettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnitySettings),
    gsettings(GnomeSettingsTool::ins())
{
    ui->setupUi(this);

    init();
}

void UnitySettings::init()
{
    bool launcherAutoHide = gsettings.getValueB(GSchemas::Unity::Shell, GSchemaKeys::Unity::LauncherHideMode, GSchemaPaths::Unity);

    GValues::RevealLocation revealLocation = (GValues::RevealLocation) gsettings.getValueI(GSchemas::Unity::Shell, GSchemaKeys::Unity::RevealTrigger, GSchemaPaths::Unity);

    float revealSensitivy = gsettings.getValueF(GSchemas::Unity::Shell, GSchemaKeys::Unity::EdgeResponsiveness, GSchemaPaths::Unity);

    bool launcherMinimzeApp = gsettings.getValueB(GSchemas::Unity::Shell, GSchemaKeys::Unity::LauncherMinimizeApp, GSchemaPaths::Unity);

    float launcherOpacity = gsettings.getValueF(GSchemas::Unity::Shell, GSchemaKeys::Unity::LauncherOpacity, GSchemaPaths::Unity);

    int launcherVisibility = gsettings.getValueI(GSchemas::Unity::Shell, GSchemaKeys::Unity::LauncherVisibility, GSchemaPaths::Unity);

    QString launcherPosition = gsettings.getValueS(GSchemas::Unity::Launcher, GSchemaKeys::Unity::LauncherPosition);

    int iconSize = gsettings.getValueI(GSchemas::Unity::Shell, GSchemaKeys::Unity::LauncherIconSize, GSchemaPaths::Unity);

    bool dashBlur = gsettings.getValueB(GSchemas::Unity::Shell, GSchemaKeys::Unity::DashBlur, GSchemaPaths::Unity);

    QString searchOnlineResource = gsettings.getValueS(GSchemas::Unity::Lens, GSchemaKeys::Unity::SearchOnlineResource);

    bool displayAvailableApps = gsettings.getValueB(GSchemas::Unity::AppLens, GSchemaKeys::Unity::DisplayAvailableApps);

    bool displayRecentApps = gsettings.getValueB(GSchemas::Unity::AppLens, GSchemaKeys::Unity::DisplayRecentApps);

    bool enableSearchFiles = gsettings.getValueB(GSchemas::Unity::FileLens, GSchemaKeys::Unity::EnableSearchFile);

    float panelOpacity = gsettings.getValueF(GSchemas::Unity::Shell, GSchemaKeys::Unity::PanelOpacity, GSchemaPaths::Unity);

    bool showDateTime = gsettings.getValueB(GSchemas::Unity::DateTime, GSchemaKeys::Unity::ShowDateTime);

    QString timeFormat = gsettings.getValueS(GSchemas::Unity::DateTime, GSchemaKeys::Unity::TimeFormat);

    bool showSeconds = gsettings.getValueB(GSchemas::Unity::DateTime, GSchemaKeys::Unity::ShowSeconds);

    bool showDate = gsettings.getValueB(GSchemas::Unity::DateTime, GSchemaKeys::Unity::ShowDate);

    bool showDay = gsettings.getValueB(GSchemas::Unity::DateTime, GSchemaKeys::Unity::ShowDay);

    bool showCalendar = gsettings.getValueB(GSchemas::Unity::DateTime, GSchemaKeys::Unity::ShowCalendar);

    bool showVolume = gsettings.getValueB(GSchemas::Unity::Sound, GSchemaKeys::Unity::ShowVolume);

    bool showShowMyName = gsettings.getValueB(GSchemas::Unity::Session, GSchemaKeys::Unity::ShowMyName);


    ui->checkLauncherAutoHide->setChecked(launcherAutoHide);

    if (revealLocation == GValues::RevealLocation::TopLeft) {
        ui->radioRevealTopLeft->setChecked(true);
    } else {
        ui->radioRevealLeft->setChecked(true);
    }

    ui->sliderRevealSensitivy->setValue(revealSensitivy / 0.1);

    ui->checkMinimizeApps->setChecked(launcherMinimzeApp);

    ui->sliderLauncherOpacity->setValue(launcherOpacity / 0.1);

    if (launcherVisibility == GValues::LauncherVisibility::AllDesktop) {
        ui->radioLauncherVisibleAllDesktop->setChecked(true);
    } else {
        ui->radioLauncherVisiblePrimaryDesktop->setChecked(true);
    }

    if (launcherPosition.contains(QRegExp("Left", Qt::CaseInsensitive))) {
        ui->radioLauncherPositionLeft->setChecked(true);
    } else {
        ui->radioLauncherPositionBottom->setChecked(true);
    }

    ui->spinIconSize->setValue(iconSize);

    ui->checkBackgroundBlur->setChecked(dashBlur);

    if (searchOnlineResource.contains(QRegExp("all", Qt::CaseInsensitive))) {
        ui->checkSearchOnlineResource->setChecked(true);
    }

    ui->checkMoreSuggestions->setChecked(displayAvailableApps);

    ui->checkRecentlyUsed->setChecked(displayRecentApps);

    ui->checkSearchYourFiles->setChecked(enableSearchFiles);

    ui->sliderPanelOpacity->setValue(panelOpacity / 0.1);

    ui->checkDateTime->setChecked(showDateTime);

    if (timeFormat.contains(QRegExp("24-hour", Qt::CaseInsensitive))) {
        ui->check24Hour->setChecked(true);
    }

    ui->checkSeconds->setChecked(showSeconds);

    ui->checkDate->setChecked(showDate);

    ui->checkWeekday->setChecked(showDay);

    ui->checkCalendar->setChecked(showCalendar);

    ui->checkVolume->setChecked(showVolume);

    ui->checkShowMyName->setChecked(showShowMyName);
}

void UnitySettings::on_checkLauncherAutoHide_clicked(bool checked)
{
    gsettings.setValueI(GSchemas::Unity::Shell, GSchemaKeys::Unity::LauncherHideMode, checked, GSchemaPaths::Unity);
}

void UnitySettings::on_radioRevealLeft_clicked()
{
    gsettings.setValueI(GSchemas::Unity::Shell, GSchemaKeys::Unity::RevealTrigger, GValues::RevealLocation::Left, GSchemaPaths::Unity);
}

void UnitySettings::on_radioRevealTopLeft_clicked()
{
    gsettings.setValueI(GSchemas::Unity::Shell, GSchemaKeys::Unity::RevealTrigger, GValues::RevealLocation::TopLeft, GSchemaPaths::Unity);
}

void UnitySettings::on_sliderRevealSensitivy_valueChanged(int value)
{
    gsettings.setValueF(GSchemas::Unity::Shell, GSchemaKeys::Unity::EdgeResponsiveness, (value * 0.1), GSchemaPaths::Unity);
}

void UnitySettings::on_checkMinimizeApps_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::Unity::Shell, GSchemaKeys::Unity::LauncherMinimizeApp, checked, GSchemaPaths::Unity);
}

void UnitySettings::on_sliderLauncherOpacity_valueChanged(int value)
{
    gsettings.setValueF(GSchemas::Unity::Shell, GSchemaKeys::Unity::LauncherOpacity, (value * 0.1), GSchemaPaths::Unity);
}

void UnitySettings::on_radioLauncherVisibleAllDesktop_clicked()
{
    gsettings.setValueI(GSchemas::Unity::Shell, GSchemaKeys::Unity::LauncherVisibility, GValues::AllDesktop, GSchemaPaths::Unity);
}

void UnitySettings::on_radioLauncherVisiblePrimaryDesktop_clicked()
{
    gsettings.setValueI(GSchemas::Unity::Shell, GSchemaKeys::Unity::LauncherVisibility, GValues::PrimaryDesktop, GSchemaPaths::Unity);
}

void UnitySettings::on_radioLauncherPositionLeft_clicked()
{
    gsettings.setValueS(GSchemas::Unity::Launcher, GSchemaKeys::Unity::LauncherPosition, "Left");
}

void UnitySettings::on_radioLauncherPositionBottom_clicked()
{
    gsettings.setValueS(GSchemas::Unity::Launcher, GSchemaKeys::Unity::LauncherPosition, "Bottom");
}

void UnitySettings::on_spinIconSize_valueChanged(int value)
{
    gsettings.setValueI(GSchemas::Unity::Shell, GSchemaKeys::Unity::LauncherIconSize, value, GSchemaPaths::Unity);
}

void UnitySettings::on_checkBackgroundBlur_clicked(bool checked)
{
    gsettings.setValueI(GSchemas::Unity::Shell, GSchemaKeys::Unity::DashBlur, checked, GSchemaPaths::Unity);
}

void UnitySettings::on_checkSearchOnlineResource_clicked(bool checked)
{
    gsettings.setValueS(GSchemas::Unity::Lens, GSchemaKeys::Unity::SearchOnlineResource, (checked ? "all" : "none"));
}

void UnitySettings::on_checkMoreSuggestions_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::Unity::AppLens, GSchemaKeys::Unity::DisplayAvailableApps, checked);
}

void UnitySettings::on_checkRecentlyUsed_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::Unity::AppLens, GSchemaKeys::Unity::DisplayRecentApps, checked);
}

void UnitySettings::on_checkSearchYourFiles_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::Unity::FileLens, GSchemaKeys::Unity::EnableSearchFile, checked);
}

void UnitySettings::on_sliderPanelOpacity_valueChanged(int value)
{
    gsettings.setValueF(GSchemas::Unity::Shell, GSchemaKeys::Unity::PanelOpacity, (value * 0.1), GSchemaPaths::Unity);
}

void UnitySettings::on_checkDateTime_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::Unity::DateTime, GSchemaKeys::Unity::ShowDateTime, checked);
}

void UnitySettings::on_check24Hour_clicked(bool checked)
{
    gsettings.setValueS(GSchemas::Unity::DateTime, GSchemaKeys::Unity::TimeFormat, (checked ? "24-hour" : "12-hour"));
}

void UnitySettings::on_checkSeconds_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::Unity::DateTime, GSchemaKeys::Unity::ShowSeconds, checked);
}

void UnitySettings::on_checkDate_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::Unity::DateTime, GSchemaKeys::Unity::ShowDate, checked);
}

void UnitySettings::on_checkWeekday_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::Unity::DateTime, GSchemaKeys::Unity::ShowDay, checked);
}

void UnitySettings::on_checkCalendar_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::Unity::DateTime, GSchemaKeys::Unity::ShowCalendar, checked);
}

void UnitySettings::on_checkVolume_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::Unity::Sound, GSchemaKeys::Unity::ShowVolume, checked);
}

void UnitySettings::on_checkShowMyName_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::Unity::Session, GSchemaKeys::Unity::ShowMyName, checked);
}
