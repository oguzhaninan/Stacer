#include "gnome_settings_page.h"
#include "ui_gnome_settings_page.h"

GnomeSettingsPage::~GnomeSettingsPage()
{
    delete ui;
}

GnomeSettingsPage::GnomeSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GnomeSettingsPage),
    slidingStackedWidget(new SlidingStackedWidget(this)),
    unitySettings(new UnitySettings(slidingStackedWidget)),
    windowManagerSettings(new WindowManagerSettings(slidingStackedWidget)),
    appearanceSettings(new AppearanceSettings(slidingStackedWidget))
{
    ui->setupUi(this);

    init();
}

void GnomeSettingsPage::init()
{
    ui->contentGridLayout->addWidget(slidingStackedWidget, 1, 0, 1, 3);

    slidingStackedWidget->addWidget(unitySettings);
    slidingStackedWidget->addWidget(windowManagerSettings);
    slidingStackedWidget->addWidget(appearanceSettings);
}

void GnomeSettingsPage::on_btnUnitySettings_clicked()
{
    slidingStackedWidget->slideInIdx(0);
}

void GnomeSettingsPage::on_btnWindowManager_clicked()
{
    slidingStackedWidget->slideInIdx(1);
}

void GnomeSettingsPage::on_btnAppearance_clicked()
{
    slidingStackedWidget->slideInIdx(2);
}
