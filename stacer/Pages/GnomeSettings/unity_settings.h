#ifndef UNITY_SETTINGS_H
#define UNITY_SETTINGS_H

#include <QWidget>
#include "Tools/gnome_settings_tool.h"

namespace Ui {
class UnitySettings;
}

class UnitySettings : public QWidget
{
    Q_OBJECT

public:
    explicit UnitySettings(QWidget *parent = 0);
    ~UnitySettings();

public:
    void init();

private slots:
    void on_checkLauncherAutoHide_toggled(bool checked);

private:
    Ui::UnitySettings *ui;

    GnomeSettingsTool gsettings;
};

#endif // UNITY_SETTINGS_H
