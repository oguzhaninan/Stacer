#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include <QApplication>
#include <QMap>
#include <QStandardPaths>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTranslator>
#include <QSystemTrayIcon>

#include "Utils/file_util.h"
#include "Managers/setting_manager.h"
#include "signal_mapper.h"

class AppManager
{

public:
    static AppManager *ins();

    QMap<QString, QString> getLanguageList() const;
    void loadLanguageList();

    QMap<QString, QString> getThemeList() const;
    void loadThemeList();

    void updateStylesheet();
    QString getStylesheetFileContent() const;

    QSettings *getStyleValues() const;

    QSystemTrayIcon *getTrayIcon();

private:
    static AppManager *instance;
    AppManager();

private:
    QTranslator translator;
    QSystemTrayIcon *mTrayIcon;

    QSettings *styleValues;

    QMap<QString, QString> languageList;
    QMap<QString, QString> themeList;
    QString stylesheetFileContent;

    SettingManager *mSettingManager;
};

#endif // APP_MANAGER_H
