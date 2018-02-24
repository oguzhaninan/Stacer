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



    ui->checkLauncherAutoHide->setChecked(launcherAutoHide);

    if (revealLocation == GValues::RevealLocation::TopLeft) {
        ui->radioRevealTopLeft->setChecked(true);
    } else {
        ui->radioRevealLeft->setChecked(true);
    }

    qDebug() << revealSensitivy << revealSensitivy/0.1;

    ui->sliderRevealSensitivy->setValue(revealSensitivy / 0.1);
}

void UnitySettings::on_checkLauncherAutoHide_toggled(bool checked)
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
