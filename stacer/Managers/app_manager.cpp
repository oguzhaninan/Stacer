#include "app_manager.h"
#include <QDebug>

AppManager *AppManager::instance = nullptr;

AppManager *AppManager::ins()
{
    if (! instance) {
        instance = new AppManager;
    }

    return instance;
}

AppManager::AppManager()
{
    mSettingManager = SettingManager::ins();

    mTrayIcon = new QSystemTrayIcon(QIcon(":/static/logo.png"));

    loadLanguageList();

    loadThemeList();

    if (translator.load(QString("stacer_%1").arg(mSettingManager->getLanguage()), qApp->applicationDirPath() + "/translations")) {
        qApp->installTranslator(&translator);
        (mSettingManager->getLanguage() == "ar") ? qApp->setLayoutDirection(Qt::RightToLeft) : qApp->setLayoutDirection(Qt::LeftToRight);
    } else {
        qCritical() << "Translator could not load.";
    }
}

QSystemTrayIcon *AppManager::getTrayIcon()
{
    return mTrayIcon;
}

QSettings *AppManager::getStyleValues() const
{
    return styleValues;
}

void AppManager::loadLanguageList()
{
    QByteArray lanuagesJson = FileUtil::readStringFromFile(":/static/languages.json").toUtf8();
    QJsonArray lanuages = QJsonDocument::fromJson(lanuagesJson).array();

    for (int i = 0; i < lanuages.count(); ++i) {

        QJsonObject ob = lanuages.at(i).toObject();

        languageList.insert(ob["value"].toString(), ob["text"].toString());
    }
}

QMap<QString, QString> AppManager::getLanguageList() const
{
    return languageList;
}

void AppManager::loadThemeList()
{
    QByteArray themesJson = FileUtil::readStringFromFile(":/static/themes.json").toUtf8();
    QJsonArray themes = QJsonDocument::fromJson(themesJson).array();

    for (int i = 0; i < themes.count(); ++i) {

        QJsonObject ob = themes.at(i).toObject();

        themeList.insert(ob["value"].toString(), ob["text"].toString());
    }
}

QMap<QString, QString> AppManager::getThemeList() const
{
    return themeList;
}

void AppManager::updateStylesheet()
{
    QString appThemePath = QString(":/static/themes/%1/style").arg(mSettingManager->getThemeName());
    styleValues = new QSettings(QString("%1/values.ini").arg(appThemePath), QSettings::IniFormat);

    stylesheetFileContent = FileUtil::readStringFromFile(QString("%1/style.qss").arg(appThemePath));

    // set values example: @color01 => #fff
    for (const QString &key : styleValues->allKeys()) {
        stylesheetFileContent.replace(key, styleValues->value(key).toString());
    }

    qApp->setStyleSheet(stylesheetFileContent);

    emit SignalMapper::ins()->changedAppTheme();
}

QString AppManager::getStylesheetFileContent() const
{
    return stylesheetFileContent;
}
