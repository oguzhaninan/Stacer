#include "app_manager.h"

AppManager *AppManager::_instance = nullptr;

AppManager *AppManager::ins()
{
    if (_instance == nullptr) {
        _instance = new AppManager;
    }

    return _instance;
}

AppManager::AppManager(QObject *parent) : QObject(parent)
{
    trayIcon = new QSystemTrayIcon(QIcon(":/static/logo.png"), this);

    // font settings
    QFontDatabase::addApplicationFont(":/static/font/Ubuntu-R.ttf");

    configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    settings = new QSettings(QString("%1/settings.conf").arg(configPath), QSettings::IniFormat);

    loadLanguageList();

    loadThemeList();

    themeName = settings->value(THEME_PROP, "default").toString();

    if (translator.load(QString("stacer_%1").arg(getLanguageCode()), qApp->applicationDirPath() + "/translations")) {
        qApp->installTranslator(&translator);
        (getLanguageCode() == "ar") ? qApp->setLayoutDirection(Qt::RightToLeft) : qApp->setLayoutDirection(Qt::LeftToRight);
    } else {
        qCritical() << "Translator could not load.";
    }

    styleValues = new QSettings(QString(":/static/themes/%1/style/values.ini").arg(themeName), QSettings::IniFormat);
}

QSystemTrayIcon *AppManager::getTrayIcon()
{
    return trayIcon;
}

QSettings *AppManager::getStyleValues() const
{
    return styleValues;
}

/************
 * LANGUAGE
 ***********/
void AppManager::loadLanguageList()
{
    QJsonArray langs = QJsonDocument::fromJson(FileUtil::readStringFromFile(":/static/languages.json").toUtf8())
            .array();

    for (int i = 0; i < langs.count(); ++i) {

        QJsonObject ob = langs.at(i).toObject();

        languageList.insert(ob["value"].toString(), ob["text"].toString());
    }
}

void AppManager::setLanguage(const QString &value)
{
    settings->setValue(LANG_PROP, value);
}

QString AppManager::getLanguageCode() const
{
    return settings->value(LANG_PROP, "en").toString();
}

QMap<QString, QString> AppManager::getLanguageList() const
{
    return languageList;
}

/************
 * THEME
 ***********/
void AppManager::loadThemeList()
{
    QJsonArray themes = QJsonDocument::fromJson(FileUtil::readStringFromFile(":/static/themes.json").toUtf8())
            .array();

    for (int i = 0; i < themes.count(); ++i) {

        QJsonObject ob = themes.at(i).toObject();

        themeList.insert(ob["value"].toString(), ob["text"].toString());
    }
}

QMap<QString, QString> AppManager::getThemeList() const
{
    return themeList;
}

QString AppManager::getThemeName() const
{
    return themeName;
}

void AppManager::updateStylesheet()
{
    styleValues = new QSettings(QString(":/static/themes/%1/style/values.ini").arg(themeName), QSettings::NativeFormat);

    stylesheetFileContent = FileUtil::readStringFromFile(QString(":/static/themes/%1/style/style.qss").arg(themeName));

    // set values
    for (const QString &key : styleValues->allKeys()) {
        stylesheetFileContent.replace(key, styleValues->value(key).toString());
    }

    qApp->setStyleSheet(stylesheetFileContent);

    emit changedTheme();
}

void AppManager::setThemeName(const QString &value)
{
    themeName = value;

    settings->setValue(THEME_PROP, value);
}

QString AppManager::getStylesheetFileContent() const
{
    return stylesheetFileContent;
}

/************
 * DISKS
 ***********/
void AppManager::setDiskName(const QString &value)
{
    settings->setValue(DISK_PROP, value);
    emit changedDisk();
}

QString AppManager::getDiskName()
{
    return settings->value(DISK_PROP, "").toString();
}

/************
 * HOMEPAGE
 ***********/

void AppManager::setHomePage(const QString &value)
{
    settings->setValue(HOMEPAGE_PROP, value);
}

QString AppManager::getHomePage()
{
    return settings->value(HOMEPAGE_PROP, "Dashboard").toString();
}

/************
 * ALERT MESSAGES
 ***********/
void AppManager::setCpuPercent(const int value)
{
    settings->setValue(CPU_PERCENT, value);
}

int AppManager::getCpuPercent()
{
    return settings->value(CPU_PERCENT, 0).toInt();
}

void AppManager::setMemoryPercent(const int value)
{
    settings->setValue(MEMORY_PERCENT, value);
}

int AppManager::getMemoryPercent()
{
    return settings->value(MEMORY_PERCENT, 0).toInt();
}

void AppManager::setDiskPercent(const int value)
{
    settings->setValue(DISK_PERCENT, value);
}

int AppManager::getDiskPercent()
{
    return settings->value(DISK_PERCENT, 0).toInt();
}
