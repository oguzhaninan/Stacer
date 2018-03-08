#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include <QApplication>
#include <QMap>
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
    QTranslator mTranslator;
    QSystemTrayIcon *mTrayIcon;

    QSettings *mStyleValues;

    QMap<QString, QString> mLanguageList;
    QMap<QString, QString> mThemeList;
    QString mStylesheetFileContent;

    SettingManager *mSettingManager;
};

#endif // APP_MANAGER_H
