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

    ui->checkLauncherAutoHide->setChecked(launcherAutoHide);
}

void UnitySettings::on_checkLauncherAutoHide_toggled(bool checked)
{
    gsettings.setValue(GSchemas::UnityShell, GSchemaKeys::LauncherHideMode, QVariant((int)checked), GSchemaPaths::Unity);
}
