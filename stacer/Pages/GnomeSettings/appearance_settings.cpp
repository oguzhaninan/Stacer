#include "appearance_settings.h"
#include "ui_appearance_settings.h"

AppearanceSettings::AppearanceSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppearanceSettings)
{
    ui->setupUi(this);
}

AppearanceSettings::~AppearanceSettings()
{
    delete ui;
}
