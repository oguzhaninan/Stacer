#ifndef WINDOW_MANAGER_SETTINGS_H
#define WINDOW_MANAGER_SETTINGS_H

#include <QWidget>
#include "Tools/gnome_settings_tool.h"

namespace Ui {
class WindowManagerSettings;
}

class WindowManagerSettings : public QWidget
{
    Q_OBJECT

public:
    explicit WindowManagerSettings(QWidget *parent = 0);
    ~WindowManagerSettings();

private slots:
    void cmbTextQuality_currentIndexChanged(int index);
    void checkWorkspaceSwitcher_clicked(bool checked);
    void spinHorizonWorkspace_valueChanged(int value);
    void spinVerticWorkspace_valueChanged(int value);
    void checkRaiseOnClick_clicked(bool checked);
    void cmbFocusMode_currentIndexChanged(QString text);
    void cmbTitleBarDoubleClick_currentIndexChanged(int index);
    void cmbTitleBarMiddleClick_currentIndexChanged(int index);
    void cmbTitleBarRightClick_currentIndexChanged(int index);
    void checkWindowSnapping_clicked(bool checked);

private:
    void init();
    void loadDatas();
    void initConnects();

private:
    Ui::WindowManagerSettings *ui;

    GnomeSettingsTool gsettings;
};

#endif // WINDOW_MANAGER_SETTINGS_H
