#include "gnome_settings_page.h"
#include "ui_gnome_settings_page.h"
#include "utilities.h"

GnomeSettingsPage::~GnomeSettingsPage()
{
    delete ui;
}

GnomeSettingsPage::GnomeSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GnomeSettingsPage),
    slidingStackedWidget(new SlidingStackedWidget(this))
{
    ui->setupUi(this);

    init();
}

void GnomeSettingsPage::init()
{
    ui->contentGridLayout->addWidget(slidingStackedWidget, 1, 0, 1, 1);

    if (GnomeSettingsTool::ins().checkUnityAvailable()) {
        unitySettings = new UnitySettings(slidingStackedWidget);
        slidingStackedWidget->addWidget(unitySettings);
    } else {
        ui->btnUnitySettings->hide();
        ui->btnWindowManager->setChecked(true);
    }

    windowManagerSettings = new WindowManagerSettings(slidingStackedWidget);
    appearanceSettings = new AppearanceSettings(slidingStackedWidget);

    slidingStackedWidget->addWidget(windowManagerSettings);
    slidingStackedWidget->addWidget(appearanceSettings);

    QList<QWidget*> widgets = { ui->btnAppearance, ui->btnUnitySettings, ui->btnWindowManager };
    Utilities::addDropShadow(widgets, 40);
}

void GnomeSettingsPage::on_btnUnitySettings_clicked()
{
    slidingStackedWidget->slideInIdx(slidingStackedWidget->indexOf(unitySettings));
}

void GnomeSettingsPage::on_btnWindowManager_clicked()
{
    slidingStackedWidget->slideInIdx(slidingStackedWidget->indexOf(windowManagerSettings));
}

void GnomeSettingsPage::on_btnAppearance_clicked()
{
    slidingStackedWidget->slideInIdx(slidingStackedWidget->indexOf(appearanceSettings));
}
