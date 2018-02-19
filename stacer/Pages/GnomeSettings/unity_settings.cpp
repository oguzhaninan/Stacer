#include "unity_settings.h"
#include "ui_unity_settings.h"

UnitySettings::UnitySettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnitySettings)
{
    ui->setupUi(this);
}

UnitySettings::~UnitySettings()
{
    delete ui;
}
