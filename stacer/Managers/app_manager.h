#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include <QApplication>
#include <QObject>
#include <QMap>
#include <QStandardPaths>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTranslator>
#include <QFontDatabase>
#include <QEasingCurve>
#include <QSystemTrayIcon>

#include <Utils/file_util.h>

#include <QDebug>

#define THEME_PROP      "ThemeName"
#define LANG_PROP       "Language"
#define DISK_PROP       "DiskName"
#define HOMEPAGE_PROP   "HomePage"
#define CPU_PERCENT     "CPUPercent"
#define MEMORY_PERCENT     "MemoryPercent"
#define DISK_PERCENT     "DiskPercent"

class AppManager : public QObject
{
    Q_OBJECT
public:
    static AppManager *ins();

    QString getLanguageCode() const;
    void setLanguage(const QString &value);
    QMap<QString, QString> getLanguageList() const;
    void loadLanguageList();
    QString getStylesheetFileContent() const;

    void updateStylesheet();
    void setThemeName(const QString &value);
    QString getThemeName() const;
    QMap<QString, QString> getThemeList() const;
    void loadThemeList();
    QSettings *getStyleValues() const;

    void setSlideType(const QEasingCurve::Type type);
    QEasingCurve::Type getSlideType() const;

    void setDiskName(const QString &value);
    QString getDiskName();

    void setHomePage(const QString &value);
    QString getHomePage();

    QSystemTrayIcon *getTrayIcon();

    void setCpuPercent(const int value);
    int getCpuPercent();

    void setMemoryPercent(const int value);
    int getMemoryPercent();

    void setDiskPercent(const int value);
    int getDiskPercent();

signals:
    void changedTheme();
    void changedSlideType(QEasingCurve::Type type);
    void changedDisk();

private:
    static AppManager *_instance;
    explicit AppManager(QObject *parent = nullptr);

private:
    QTranslator translator;
    QString configPath;
    QString themeName;

    QSystemTrayIcon *trayIcon;

    enum QEasingCurve::Type slideType;

    QSettings *settings;
    QSettings *styleValues;

    QMap<QString, QString> languageList;
    QMap<QString, QString> themeList;
    QString stylesheetFileContent;
};

#endif // APP_MANAGER_H
