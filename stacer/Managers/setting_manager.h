#ifndef SETTING_MANAGER_H
#define SETTING_MANAGER_H

#include <QSettings>
#include <QStandardPaths>

namespace SettingKeys {
    const QString ThemeName("ThemeName");
    const QString Language("Language");
    const QString DiskName("DiskName");
    const QString StartPage("StartPage");
    const QString CPUAlertPercent("CPUAlertPercent");
    const QString MemoryAlertPercent("MemoryAlertPercent");
    const QString DiskAlertPercent("DiskAlertPercent");
    const QString GPUMemoryAlertPercent("GPUMemoryAlertPercent");
    const QString GPUUsageAlertPercent("GPUUsageAlertPercent");
    const QString AppQuitDialogDontAsk("AppQuitDialogDontAsk");
    const QString AppQuitDialogChoice("AppQuitDialogChoice");
}

class SettingManager
{
public:
    static SettingManager *ins();

    QString getConfigPath() const;

    void setLanguage(const QString &value);
    QString getLanguage() const;

    void setThemeName(const QString &value);
    QString getThemeName() const;

    void setDiskName(const QString &value);
    QString getDiskName() const;

    void setStartPage(const QString &value);
    QString getStartPage() const;

    void setCpuAlertPercent(const int value);
    int getCpuAlertPercent() const;

    void setMemoryAlertPercent(const int value);
    int getMemoryAlertPercent() const;

    void setDiskAlertPercent(const int value);
    int getDiskAlertPercent() const;

    void setGPUMemoryAlertPercent(const int value);
    int getGPUMemoryAlertPercent() const;

    void setGPUUsageAlertPercent(const int value);
    int getGPUUsageAlertPercent() const;

    void setAppQuitDialogDontAsk(const bool value);
    bool getAppQuitDialogDontAsk() const;

    void setAppQuitDialogChoice(const QString &value);
    QString getAppQuitDialogChoice() const;

private:
    static SettingManager *instance;
    SettingManager();

    QSettings *mSettings;
    QString mConfigPath;
};

#endif // SETTING_MANAGER_H
