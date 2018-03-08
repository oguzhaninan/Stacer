#include "appearance_settings.h"
#include "ui_appearance_settings.h"
#include <QDebug>
AppearanceSettings::~AppearanceSettings()
{
    delete ui;
}

AppearanceSettings::AppearanceSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppearanceSettings),
    gsettings(GnomeSettingsTool::ins())
{
    ui->setupUi(this);

    loadDatas();

    init();

    initConnects();
}

void AppearanceSettings::init()
{
    bool showDesktopIcons = gsettings.getValueB(GSchemas::Appearance::Background, GSchemaKeys::Appearance::ShowDesktopIcons);

    bool showHomeIcon = gsettings.getValueB(GSchemas::Appearance::Desktop, GSchemaKeys::Appearance::ShowHomeIcon);

    bool showNetworkIcon = gsettings.getValueB(GSchemas::Appearance::Desktop, GSchemaKeys::Appearance::ShowNetworkIcon);

    bool showTrashIcon = gsettings.getValueB(GSchemas::Appearance::Desktop, GSchemaKeys::Appearance::ShowTrashIcon);

    bool showVolumesIcon = gsettings.getValueB(GSchemas::Appearance::Desktop, GSchemaKeys::Appearance::ShowVolumesIcon);

    QString desktopBackMode = gsettings.getValueS(GSchemas::Appearance::Background, GSchemaKeys::Appearance::PictureOptions).replace("'","");

    QString loginBackMode = gsettings.getValueS(GSchemas::Appearance::Screensaver, GSchemaKeys::Appearance::PictureOptions).replace("'","");

    bool screenKeyboardEnabled = gsettings.getValueB(GSchemas::Appearance::Applications, GSchemaKeys::Appearance::ScreenKeyboard);

    bool screenReaderEnabled = gsettings.getValueB(GSchemas::Appearance::Applications, GSchemaKeys::Appearance::ScreenReader);

    ui->checkShowDesktopIcons->setChecked(showDesktopIcons);

    ui->checkHomeIcon->setChecked(showHomeIcon);

    ui->checkNetworkIcon->setChecked(showNetworkIcon);

    ui->checkTrashIcon->setChecked(showTrashIcon);

    ui->checkMountedVulmesIcon->setChecked(showVolumesIcon);

    ui->cmbDesktopBackMode->setCurrentIndex(ui->cmbDesktopBackMode->findData(desktopBackMode));

    ui->cmbLoginBackMode->setCurrentIndex(ui->cmbLoginBackMode->findData(loginBackMode));

    ui->checkScreenKeyboard->setChecked(screenKeyboardEnabled);

    ui->checkScreenReader->setChecked(screenReaderEnabled);
}

void AppearanceSettings::initConnects()
{
    connect(ui->cmbDesktopBackMode, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbDesktopBackMode_currentIndexChanged(int)));
    connect(ui->cmbLoginBackMode, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbLoginBackMode_currentIndexChanged(int)));
}

void AppearanceSettings::loadDatas()
{
    QList<QPair<QString, QString>> backgroundModes = {
        {tr("None"), "none"}, {tr("Wallpaper"), "wallpaper"}, {tr("Centered"), "centered"},
        {tr("Scaled"), "scaled"}, {tr("Stretched"), "stretched"}, {tr("Zoom"), "zoom"}, {tr("Spanned"), "spanned"}
    };

    for (const QPair<QString, QString> &mode : backgroundModes) {
        ui->cmbDesktopBackMode->addItem(mode.first, mode.second);
        ui->cmbLoginBackMode->addItem(mode.first, mode.second);
    }
}

void AppearanceSettings::on_checkShowDesktopIcons_clicked(bool checked)
{
    if (! checked) {
        ui->checkHomeIcon->setChecked(checked);
        ui->checkNetworkIcon->setChecked(checked);
        ui->checkMountedVulmesIcon->setChecked(checked);
        ui->checkTrashIcon->setChecked(checked);
    }
    gsettings.setValueB(GSchemas::Appearance::Background, GSchemaKeys::Appearance::ShowDesktopIcons, checked);
}

void AppearanceSettings::on_checkHomeIcon_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::Appearance::Desktop, GSchemaKeys::Appearance::ShowHomeIcon, checked);
}

void AppearanceSettings::on_checkTrashIcon_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::Appearance::Desktop, GSchemaKeys::Appearance::ShowTrashIcon, checked);
}

void AppearanceSettings::on_checkMountedVulmesIcon_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::Appearance::Desktop, GSchemaKeys::Appearance::ShowVolumesIcon, checked);
}

void AppearanceSettings::on_checkNetworkIcon_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::Appearance::Desktop, GSchemaKeys::Appearance::ShowNetworkIcon, checked);
}

void AppearanceSettings::cmbDesktopBackMode_currentIndexChanged(int index)
{
    QString data = ui->cmbDesktopBackMode->itemData(index).toString();
    gsettings.setValueS(GSchemas::Appearance::Background, GSchemaKeys::Appearance::PictureOptions, data);
}

void AppearanceSettings::cmbLoginBackMode_currentIndexChanged(int index)
{
    QString data = ui->cmbLoginBackMode->itemData(index).toString();
    gsettings.setValueS(GSchemas::Appearance::Screensaver, GSchemaKeys::Appearance::PictureOptions, data);
}

void AppearanceSettings::on_checkScreenKeyboard_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::Appearance::Applications, GSchemaKeys::Appearance::ScreenKeyboard, checked);
}

void AppearanceSettings::on_checkScreenReader_clicked(bool checked)
{
    gsettings.setValueB(GSchemas::Appearance::Applications, GSchemaKeys::Appearance::ScreenReader, checked);
}
