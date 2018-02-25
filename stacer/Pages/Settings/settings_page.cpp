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
        ui->disksCmb->addItem(QString("%1  (%2)").arg(disk->device).arg(disk->name), disk->name);
    }

    QString dk = apm->getDiskName();
    ui->disksCmb->setCurrentText(dk);

    // start on boot
    startupAppPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    startupAppPath.append("/autostart/stacer.desktop");

    QFile startupAppFile(startupAppPath);
    if (startupAppFile.exists()) {
        QStringList appContent = FileUtil::readListFromFile(startupAppPath);
        QString isHidden = Utilities::getDesktopValue(QRegExp("^Hidden=.*"), appContent).toLower();
        ui->checkAutostart->setChecked(isHidden == "false");
    } else {
        ui->checkAutostart->setChecked(false);
    }

    // effects
    Utilities::addDropShadow(ui->languagesCmb, 40);
    Utilities::addDropShadow(ui->themesCmb, 40);
    Utilities::addDropShadow(ui->disksCmb, 40);

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

void SettingsPage::on_checkAutostart_clicked(bool checked)
{
    if (checked) {
        QString appTemplate = QString("[Desktop Entry]\n"
                                      "Name=Stacer\n"
                                      "Comment=Linux System Optimizer and Monitoring\n"
                                      "Exec=/usr/share/stacer/stacer -m \n"
                                      "Type=Application\n"
                                      "Terminal=false\n"
                                      "Hidden=false");

        QFile autostartApp(startupAppPath);

        if (autostartApp.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream stream(&autostartApp);
            stream << appTemplate << endl;

            autostartApp.close();
        }
    } else {
        QFile::remove(startupAppPath);
    }
}
