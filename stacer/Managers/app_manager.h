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

#include <Utils/file_util.h>

#include <QDebug>

#define THEME_PROP "ThemeName"
#define LANG_PROP "Language"

class AppManager : public QObject
{
    Q_OBJECT
public:
    static AppManager *ins();

    QString getLanguageCode();
    void setLanguage(QString value);
    QMap<QString, QString> getLanguageList();
    void loadLanguageList();
    QString getStylesheetFileContent() const;

    void updateStylesheet();
    void setThemeName(const QString &value);
    QString getThemeName() const;
    QMap<QString, QString> getThemeList() const;
    void loadThemeList();
    QSettings *getStyleValues() const;

signals:
    void changedTheme();

private:
    static AppManager *_instance;
    explicit AppManager(QObject *parent = nullptr);

private:
    QTranslator translator;
    QString configPath;
    QString themeName;
    QSettings *settings;
    QSettings *styleValues;
    QMap<QString, QString> languageList;
    QMap<QString, QString> themeList;
    QString stylesheetFileContent;
};

#endif // APP_MANAGER_H
