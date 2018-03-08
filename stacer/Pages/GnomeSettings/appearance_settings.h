#ifndef APPEARANCE_SETTINGS_H
#define APPEARANCE_SETTINGS_H

#include <QWidget>
#include "Tools/gnome_settings_tool.h"

namespace Ui {
class AppearanceSettings;
}

class AppearanceSettings : public QWidget
{
    Q_OBJECT

public:
    explicit AppearanceSettings(QWidget *parent = 0);
    ~AppearanceSettings();

private slots:
    void on_checkShowDesktopIcons_clicked(bool checked);
    void on_checkHomeIcon_clicked(bool checked);
    void on_checkTrashIcon_clicked(bool checked);
    void on_checkMountedVulmesIcon_clicked(bool checked);
    void on_checkNetworkIcon_clicked(bool checked);
    void cmbDesktopBackMode_currentIndexChanged(int index);
    void cmbLoginBackMode_currentIndexChanged(int index);
    void on_checkScreenKeyboard_clicked(bool checked);
    void on_checkScreenReader_clicked(bool checked);

private:
    void init();
    void initConnects();
    void loadDatas();

private:
    Ui::AppearanceSettings *ui;

    GnomeSettingsTool gsettings;
};

#endif // APPEARANCE_SETTINGS_H
