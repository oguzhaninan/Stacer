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

    ui->checkShowDesktopIcons->setChecked(showDesktopIcons);

    ui->checkHomeIcon->setChecked(showHomeIcon);

    ui->checkNetworkIcon->setChecked(showNetworkIcon);

    ui->checkTrashIcon->setChecked(showTrashIcon);

    ui->checkMountedVulmesIcon->setChecked(showVolumesIcon);

    ui->cmbDesktopBackMode->setCurrentIndex(ui->cmbDesktopBackMode->findData(desktopBackMode));

    ui->cmbLoginBackMode->setCurrentIndex(ui->cmbLoginBackMode->findData(loginBackMode));
}

void AppearanceSettings::initConnects()
{
    connect(ui->cmbDesktopBackMode, SIGNAL(currentIndexChanged(QString)), this, SLOT(cmbDesktopBackMode_currentIndexChanged(QString)));
    connect(ui->cmbLoginBackMode, SIGNAL(currentIndexChanged(QString)), this, SLOT(cmbLoginBackMode_currentIndexChanged(QString)));
}

void AppearanceSettings::loadDatas()
{
    QStringList backgroundModes = { tr("None"), tr("Wallpaper"), tr("Centered"),
                                    tr("Scaled"), tr("Stretched"), tr("Zoom"), tr("Spanned") };

    for (const QString &mode : backgroundModes) {
        ui->cmbDesktopBackMode->addItem(mode, mode.toLower());
        ui->cmbLoginBackMode->addItem(mode, mode.toLower());
    }
}

void AppearanceSettings::on_checkShowDesktopIcons_clicked(bool checked)
{
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

void AppearanceSettings::cmbDesktopBackMode_currentIndexChanged(const QString &text)
{
    gsettings.setValueS(GSchemas::Appearance::Background, GSchemaKeys::Appearance::PictureOptions, text.toLower());
}

void AppearanceSettings::cmbLoginBackMode_currentIndexChanged(const QString &text)
{
    gsettings.setValueS(GSchemas::Appearance::Screensaver, GSchemaKeys::Appearance::PictureOptions, text.toLower());
}
