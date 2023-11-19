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

private:
    static SettingManager *instance;
    SettingManager();

    QSettings *mSettings;
    QString mConfigPath;
};

#endif // SETTING_MANAGER_H
