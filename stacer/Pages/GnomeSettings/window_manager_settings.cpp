#include "window_manager_settings.h"
#include "ui_window_manager_settings.h"
#include <QDebug>

WindowManagerSettings::~WindowManagerSettings()
{
    delete ui;
}

WindowManagerSettings::WindowManagerSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WindowManagerSettings),
    gsettings(GnomeSettingsTool::ins())
{
    ui->setupUi(this);

    loadDatas();

    init();

    initConnects();
}

void WindowManagerSettings::init()
{
    int textureFilter = gsettings.getValueI(GSchemas::Window::OpenGL, GSchemaKeys::Window::TextureQuality, GSchemaPaths::OpenGL);

    int horizontalWorkspaceSize = gsettings.getValueI(GSchemas::Window::Core, GSchemaKeys::Window::HorizontalWorkSize, GSchemaPaths::Core);

    int verticalWorkspaceSize = gsettings.getValueI(GSchemas::Window::Core, GSchemaKeys::Window::VerticalWorkSize, GSchemaPaths::Core);

    bool workspaceSwitcher = horizontalWorkspaceSize > 1 || verticalWorkspaceSize > 1;

    bool raiseOnClick = gsettings.getValueI(GSchemas::Window::Preferences, GSchemaKeys::Window::RaiseOnClick);

    QString focusMode = gsettings.getValueS(GSchemas::Window::Preferences, GSchemaKeys::Window::FocusMode).replace("'", "");

    QString actionDoubleClick = gsettings.getValueS(GSchemas::Window::Preferences, GSchemaKeys::Window::ActionDoubleClick).replace("'", "");

    QString actionMiddleClick = gsettings.getValueS(GSchemas::Window::Preferences, GSchemaKeys::Window::ActionMiddleClick).replace("'", "");

    QString actionRightClick = gsettings.getValueS(GSchemas::Window::Preferences, GSchemaKeys::Window::ActionRightClick).replace("'", "");


    ui->cmbTextQuality->setCurrentIndex(textureFilter);

    ui->checkWorkspaceSwitcher->setChecked(workspaceSwitcher);

    ui->spinHorizonWorkspace->setValue(horizontalWorkspaceSize);

    ui->spinVerticWorkspace->setValue(verticalWorkspaceSize);

    ui->checkRaiseOnClick->setChecked(raiseOnClick);

    ui->cmbFocusMode->setCurrentText(focusMode);

    ui->cmbTitleBarDoubleClick->setCurrentIndex(ui->cmbTitleBarDoubleClick->findData(actionDoubleClick));

    ui->cmbTitleBarMiddleClick->setCurrentIndex(ui->cmbTitleBarMiddleClick->findData(actionMiddleClick));

    ui->cmbTitleBarRightClick->setCurrentIndex(ui->cmbTitleBarRightClick->findData(actionRightClick));
}

void WindowManagerSettings::loadDatas()
{
    ui->cmbTextQuality->addItems({tr("Fast"), tr("Good"), tr("Best")});

    ui->cmbFocusMode->addItems({tr("Click"), tr("Sloppy"), tr("Mouse")});

    QList<QPair<QString, QString>> titleBarClickActions = {
        {tr("Toggle Shade"), "toggle-shade"}, {tr("Maximize"), "toggle-maximize"}, {tr("Maximize Horizontally"), "toggle-maximize-horizontally"},
        {tr("Maximize Vertically"), "toggle-maximize-vertically"}, {tr("Minimize"), "minimize"},
        {tr("none"), "none"}, {tr("Lower"), "lower"}, {tr("Menu"), "menu"}
    };

    for (const QPair<QString, QString> &action : titleBarClickActions) {
        ui->cmbTitleBarDoubleClick->addItem(action.first, action.second);
        ui->cmbTitleBarMiddleClick->addItem(action.first, action.second);
        ui->cmbTitleBarRightClick-> addItem(action.first, action.second);
    }
}

void WindowManagerSettings::initConnects()
{
    connect(ui->cmbTextQuality, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbTextQuality_currentIndexChanged(int)));
    connect(ui->checkWorkspaceSwitcher, SIGNAL(clicked(bool)), this, SLOT(checkWorkspaceSwitcher_clicked(bool)));
    connect(ui->spinHorizonWorkspace, SIGNAL(valueChanged(int)), this, SLOT(spinHorizonWorkspace_valueChanged(int)));
    connect(ui->spinVerticWorkspace, SIGNAL(valueChanged(int)), this, SLOT(spinVerticWorkspace_valueChanged(int)));
    connect(ui->checkRaiseOnClick, SIGNAL(clicked(bool)), this, SLOT(checkRaiseOnClick_clicked(bool)));
    connect(ui->cmbFocusMode, SIGNAL(currentIndexChanged(QString)), this, SLOT(cmbFocusMode_currentIndexChanged(QString)));
    connect(ui->cmbTitleBarDoubleClick, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbTitleBarDoubleClick_currentIndexChanged(int)));
    connect(ui->cmbTitleBarMiddleClick, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbTitleBarMiddleClick_currentIndexChanged(int)));
    connect(ui->cmbTitleBarRightClick, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbTitleBarRightClick_currentIndexChanged(int)));
}

void WindowManagerSettings::cmbTextQuality_currentIndexChanged(int index)
{
    gsettings.setValueI(GSchemas::Window::OpenGL, GSchemaKeys::Window::TextureQuality, index, GSchemaPaths::OpenGL);
}

void WindowManagerSettings::checkWorkspaceSwitcher_clicked(bool checked)
{
    int workSize = checked ? 2 : 1;

    gsettings.setValueI(GSchemas::Window::Core, GSchemaKeys::Window::HorizontalWorkSize, workSize, GSchemaPaths::Core);
    gsettings.setValueI(GSchemas::Window::Core, GSchemaKeys::Window::VerticalWorkSize, workSize, GSchemaPaths::Core);

    ui->spinHorizonWorkspace->setValue(workSize);
    ui->spinVerticWorkspace->setValue(workSize);
}

void WindowManagerSettings::spinHorizonWorkspace_valueChanged(int value)
{
    gsettings.setValueI(GSchemas::Window::Core, GSchemaKeys::Window::HorizontalWorkSize, value, GSchemaPaths::Core);
}

void WindowManagerSettings::spinVerticWorkspace_valueChanged(int value)
{
    gsettings.setValueI(GSchemas::Window::Core, GSchemaKeys::Window::VerticalWorkSize, value, GSchemaPaths::Core);
}

void WindowManagerSettings::checkRaiseOnClick_clicked(bool checked)
{
    gsettings.setValueI(GSchemas::Window::Preferences, GSchemaKeys::Window::RaiseOnClick, checked);
}

void WindowManagerSettings::cmbFocusMode_currentIndexChanged(QString text)
{
    gsettings.setValueS(GSchemas::Window::Preferences, GSchemaKeys::Window::FocusMode, text.toLower());
}

void WindowManagerSettings::cmbTitleBarDoubleClick_currentIndexChanged(int index)
{
    QString data = ui->cmbTitleBarDoubleClick->itemData(index).toString();
    gsettings.setValueS(GSchemas::Window::Preferences, GSchemaKeys::Window::ActionDoubleClick, data);
}

void WindowManagerSettings::cmbTitleBarMiddleClick_currentIndexChanged(int index)
{
    QString data = ui->cmbTitleBarMiddleClick->itemData(index).toString();
    gsettings.setValueS(GSchemas::Window::Preferences, GSchemaKeys::Window::ActionMiddleClick, data);
}

void WindowManagerSettings::cmbTitleBarRightClick_currentIndexChanged(int index)
{
    QString data = ui->cmbTitleBarRightClick->itemData(index).toString();
    gsettings.setValueS(GSchemas::Window::Preferences, GSchemaKeys::Window::ActionRightClick, data);
}
