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
    bool launcherAutoHide = gsettings.getValueB(GSchemas::UnityShell, GSchemaKeys::LauncherHideMode, GSchemaPaths::Unity);

    GValues::RevealLocation revealLocation = (GValues::RevealLocation) gsettings.getValueI(GSchemas::UnityShell, GSchemaKeys::RevealTrigger, GSchemaPaths::Unity);

    float revealSensitivy = gsettings.getValueF(GSchemas::UnityShell, GSchemaKeys::EdgeResponsiveness, GSchemaPaths::Unity);

    bool launcherMinimzeApp = gsettings.getValueB(GSchemas::UnityShell, GSchemaKeys::LauncherMinimizeApp, GSchemaPaths::Unity);

    float launcherOpacity = gsettings.getValueF(GSchemas::UnityShell, GSchemaKeys::LauncherOpacity, GSchemaPaths::Unity);

    GValues::LauncherVisibility launcherVisibility = (GValues::LauncherVisibility) gsettings.getValueI(GSchemas::UnityShell, GSchemaKeys::LauncherVisibility, GSchemaPaths::Unity);

    QString launcherPosition = gsettings.getValueS(GSchemas::UnityLauncher, GSchemaKeys::LauncherPosition);

//    bool disableShowDesktop = gsettings.getValueB(GSchemas::UnityShell, GSchemaKeys::DisableShowDesktop, GSchemaPaths::Unity);

    int iconSize = gsettings.getValueI(GSchemas::UnityShell, GSchemaKeys::LauncherIconSize, GSchemaPaths::Unity);


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

//    ui->checkShowDesktopIcon->setChecked(!disableShowDesktop);

    ui->spinIconSize->setValue(iconSize);
}

void UnitySettings::on_checkLauncherAutoHide_clicked(bool checked)
{
    gsettings.setValueI(GSchemas::UnityShell, GSchemaKeys::LauncherHideMode, checked, GSchemaPaths::Unity);
}

void UnitySettings::on_radioRevealLeft_clicked()
{
    gsettings.setValueI(GSchemas::UnityShell, GSchemaKeys::RevealTrigger, GValues::RevealLocation::Left, GSchemaPaths::Unity);
}

void UnitySettings::on_radioRevealTopLeft_clicked()
{
    gsettings.setValueI(GSchemas::UnityShell, GSchemaKeys::RevealTrigger, GValues::RevealLocation::TopLeft, GSchemaPaths::Unity);
}

void UnitySettings::on_sliderRevealSensitivy_valueChanged(int value)
{
    gsettings.setValueF(GSchemas::UnityShell, GSchemaKeys::EdgeResponsiveness, (value * 0.1), GSchemaPaths::Unity);
}

void UnitySettings::on_checkMinimizeApps_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::UnityShell, GSchemaKeys::LauncherMinimizeApp, checked, GSchemaPaths::Unity);
}

void UnitySettings::on_sliderLauncherOpacity_valueChanged(int value)
{
    gsettings.setValueF(GSchemas::UnityShell, GSchemaKeys::LauncherOpacity, (value * 0.1), GSchemaPaths::Unity);
}

void UnitySettings::on_radioLauncherVisibleAllDesktop_clicked()
{
    gsettings.setValueI(GSchemas::UnityShell, GSchemaKeys::LauncherVisibility, GValues::AllDesktop, GSchemaPaths::Unity);
}

void UnitySettings::on_radioLauncherVisiblePrimaryDesktop_clicked()
{
    gsettings.setValueI(GSchemas::UnityShell, GSchemaKeys::LauncherVisibility, GValues::PrimaryDesktop, GSchemaPaths::Unity);
}

void UnitySettings::on_radioLauncherPositionLeft_clicked()
{
    gsettings.setValueS(GSchemas::UnityLauncher, GSchemaKeys::LauncherPosition, "Left");
}

void UnitySettings::on_radioLauncherPositionBottom_clicked()
{
    gsettings.setValueS(GSchemas::UnityLauncher, GSchemaKeys::LauncherPosition, "Bottom");
}

//void UnitySettings::on_checkShowDesktopIcon_clicked(bool checked)
//{
//    gsettings.setValueB(GSchemas::UnityShell, GSchemaKeys::DisableShowDesktop, !checked, GSchemaPaths::Unity);
//}

void UnitySettings::on_spinIconSize_valueChanged(int value)
{
    gsettings.setValueI(GSchemas::UnityShell, GSchemaKeys::LauncherIconSize, value, GSchemaPaths::Unity);
}
