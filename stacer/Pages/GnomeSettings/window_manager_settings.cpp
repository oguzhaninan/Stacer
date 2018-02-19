#include "window_manager_settings.h"
#include "ui_window_manager_settings.h"

WindowManagerSettings::WindowManagerSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WindowManagerSettings)
{
    ui->setupUi(this);
}

WindowManagerSettings::~WindowManagerSettings()
{
    delete ui;
}
