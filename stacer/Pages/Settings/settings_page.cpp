#include "settings_page.h"
#include "ui_settings_page.h"
#include "Managers/info_manager.h"
#include "utilities.h"
#include <QDesktopServices>
#include <QUrl>

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
        ui->cmbLanguages->addItem(lang.value(), lang.key());
    }

    QString lc = apm->getLanguageCode();
    ui->cmbLanguages->setCurrentText(apm->getLanguageList().value(lc));

    // load themes
    QMapIterator<QString, QString> theme(apm->getThemeList());

    while (theme.hasNext()) {
        theme.next();
        ui->cmbThemes->addItem(theme.value(), theme.key());
    }

    QString tn = apm->getThemeName();
    ui->cmbThemes->setCurrentText(apm->getThemeList().value(tn));

    // load disks
    InfoManager::ins()->updateDiskInfo();
    QList<Disk*> disks = InfoManager::ins()->getDisks();

    for (const Disk *disk : disks) {
        ui->cmbDisks->addItem(QString("%1  (%2)").arg(disk->device).arg(disk->name), disk->name);
    }

    QString dk = apm->getDiskName();
    ui->cmbDisks->setCurrentIndex(ui->cmbDisks->findData(dk));

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

    // load pages
    ui->cmbHomepage->addItems({
        tr("Dashboard"), tr("Startup Apps"), tr("System Cleaner"),
        tr("Services"), tr("Processes"), tr("Uninstaller"), tr("Resources")
    });

    ui->cmbHomepage->setCurrentText(apm->getHomePage());

    // load resource percents
    ui->spinCpuPercent->setValue(apm->getCpuPercent());
    ui->spinMemoryPercent->setValue(apm->getMemoryPercent());
    ui->spinDiskPercent->setValue(apm->getDiskPercent());

    // effects
    QList<QWidget*> widgets = {
        ui->cmbLanguages, ui->cmbThemes, ui->cmbDisks, ui->cmbHomepage, ui->btnDonate, ui->spinCpuPercent,
        ui->spinMemoryPercent, ui->spinDiskPercent
    };

    Utilities::addDropShadow(widgets, 50);

    // connects
    connect(ui->cmbLanguages, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbLanguagesChanged(int)));
    connect(ui->cmbThemes, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbThemesChanged(int)));    
    connect(ui->cmbDisks, SIGNAL(currentIndexChanged(int)), this, SLOT(diskCmbChanged(int)));
    connect(ui->cmbHomepage, SIGNAL(currentIndexChanged(QString)), this, SLOT(cmbHomePageChanged(QString)));
}

void SettingsPage::cmbLanguagesChanged(const int &index)
{
    QString langCode = ui->cmbLanguages->itemData(index).toString();

    apm->setLanguage(langCode);
}

void SettingsPage::cmbThemesChanged(const int &index)
{
    QString themeName = ui->cmbThemes->itemData(index).toString();

    apm->setThemeName(themeName);
    apm->updateStylesheet();
}

void SettingsPage::diskCmbChanged(const int &index)
{
    QString diskName = ui->cmbDisks->itemData(index).toString();

    apm->setDiskName(diskName);
}

void SettingsPage::on_checkAutostart_clicked(bool checked)
{
    if (checked) {
        QString appTemplate = QString("[Desktop Entry]\n"
                                      "Name=Stacer\n"
                                      "Comment=Linux System Optimizer and Monitoring\n"
                                      "Exec=/usr/share/stacer/stacer --hide \n"
                                      "Type=Application\n"
                                      "Terminal=false\n"
                                      "Hidden=false\n");

        FileUtil::writeFile(startupAppPath, appTemplate);
    } else {
        QFile::remove(startupAppPath);
    }
}

void SettingsPage::on_btnDonate_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.patreon.com/oguzhaninan"));
}

void SettingsPage::cmbHomePageChanged(const QString text)
{
    apm->setHomePage(text);
}

void SettingsPage::on_spinCpuPercent_valueChanged(int value)
{
    apm->setCpuPercent(value);
}

void SettingsPage::on_spinMemoryPercent_valueChanged(int value)
{
    apm->setMemoryPercent(value);
}

void SettingsPage::on_spinDiskPercent_valueChanged(int value)
{
    apm->setDiskPercent(value);
}
