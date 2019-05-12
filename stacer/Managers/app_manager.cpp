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

    mTrayIcon = new QSystemTrayIcon(QIcon(":/static/themes/default/img/sidebar-icons/dash.png"));

    loadLanguageList();

//    loadThemeList();

    if (mTranslator.load(QString("stacer_%1").arg(mSettingManager->getLanguage()), qApp->applicationDirPath() + "/translations")) {
        qApp->installTranslator(&mTranslator);
        (mSettingManager->getLanguage() == "ar") ? qApp->setLayoutDirection(Qt::RightToLeft) : qApp->setLayoutDirection(Qt::LeftToRight);
    }
}

QSystemTrayIcon *AppManager::getTrayIcon()
{
    return mTrayIcon;
}

QSettings *AppManager::getStyleValues() const
{
    return mStyleValues;
}

void AppManager::loadLanguageList()
{
    QByteArray lanuagesJson = FileUtil::readStringFromFile(":/static/languages.json").toUtf8();
    QJsonArray lanuages = QJsonDocument::fromJson(lanuagesJson).array();

    for (int i = 0; i < lanuages.count(); ++i) {

        QJsonObject ob = lanuages.at(i).toObject();

        mLanguageList.insert(ob["value"].toString(), ob["text"].toString());
    }
}

QMap<QString, QString> AppManager::getLanguageList() const
{
    return mLanguageList;
}

//void AppManager::loadThemeList()
//{
//    QByteArray themesJson = FileUtil::readStringFromFile(":/static/themes.json").toUtf8();
//    QJsonArray themes = QJsonDocument::fromJson(themesJson).array();

//    for (int i = 0; i < themes.count(); ++i) {

//        QJsonObject ob = themes.at(i).toObject();

//        mThemeList.insert(ob["value"].toString(), ob["text"].toString());
//    }
//}

//QMap<QString, QString> AppManager::getThemeList() const
//{
//    return mThemeList;
//}

void AppManager::updateStylesheet()
{
    QString appThemePath = QString(":/static/themes/%1/style").arg(mSettingManager->getThemeName());
    mStyleValues = new QSettings(QString("%1/values.ini").arg(appThemePath), QSettings::IniFormat);

    mStylesheetFileContent = FileUtil::readStringFromFile(QString("%1/style.qss").arg(appThemePath));

    // set values example: @color01 => #fff
    for (const QString &key : mStyleValues->allKeys()) {
        mStylesheetFileContent.replace(key, mStyleValues->value(key).toString());
    }

    qApp->setStyleSheet(mStylesheetFileContent);

    emit SignalMapper::ins()->sigChangedAppTheme();
}

QString AppManager::getStylesheetFileContent() const
{
    return mStylesheetFileContent;
}
