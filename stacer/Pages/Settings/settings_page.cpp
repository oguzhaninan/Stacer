#include "settings_page.h"
#include "ui_settings_page.h"
#include "Managers/info_manager.h"
#include "utilities.h"

SettingsPage::~SettingsPage()
{
    delete ui;
}

SettingsPage::SettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsPage),
    apm(AppManager::ins())
{
    ui->setupUi(this);

    init();
}

void SettingsPage::init()
{
    // load languages
    QMapIterator<QString, QString> lang(apm->getLanguageList());

    while (lang.hasNext()) {
        lang.next();
        ui->languagesCmb->addItem(lang.value(), lang.key());
    }

    QString lc = apm->getLanguageCode();
    ui->languagesCmb->setCurrentText(apm->getLanguageList().value(lc));

    // load themes
    QMapIterator<QString, QString> theme(apm->getThemeList());

    while (theme.hasNext()) {
        theme.next();
        ui->themesCmb->addItem(theme.value(), theme.key());
    }

    QString tn = apm->getThemeName();
    ui->themesCmb->setCurrentText(apm->getThemeList().value(tn));

    // load disks
    InfoManager::ins()->updateDiskInfo();
    QList<Disk*> disks = InfoManager::ins()->getDisks();

    for (const Disk *disk : disks) {
        ui->disksCmb->addItem(disk->name, disk->name);
    }

    QString dk = apm->getDiskName();
    ui->disksCmb->setCurrentText(dk);

    // effects
    Utilities::addDropShadow(ui->languagesCmb, 55);
    Utilities::addDropShadow(ui->themesCmb, 55);
    Utilities::addDropShadow(ui->disksCmb, 55);

    connect(ui->languagesCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(languagesCmbChanged(int)));
    connect(ui->themesCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(themesCmbChanged(int)));    
    connect(ui->disksCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(diskCmbChanged(int)));
}

void SettingsPage::languagesCmbChanged(const int &index)
{
    QString langCode = ui->languagesCmb->itemData(index).toString();

    apm->setLanguage(langCode);
}

void SettingsPage::themesCmbChanged(const int &index)
{
    QString themeName = ui->themesCmb->itemData(index).toString();

    apm->setThemeName(themeName);
    apm->updateStylesheet();
}

void SettingsPage::diskCmbChanged(const int &index)
{
    QString diskName = ui->disksCmb->itemData(index).toString();

    apm->setDiskName(diskName);
}
