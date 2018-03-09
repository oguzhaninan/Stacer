#include "uninstaller_page.h"
#include "ui_uninstallerpage.h"
#include <QMovie>
#include "utilities.h"

UninstallerPage::~UninstallerPage()
{
    delete ui;
}

UninstallerPage::UninstallerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UninstallerPage),
    tm(ToolManager::ins())
{
    ui->setupUi(this);

    init();
}

void UninstallerPage::init()
{
    QString iconLoading = QString(":/static/themes/%1/img/loading.gif").arg(SettingManager::ins()->getThemeName());
    QMovie *loadingMovie = new QMovie(iconLoading, QByteArray(), this);
    ui->lblLoadingUninstaller->setMovie(loadingMovie);
    loadingMovie->start();
    ui->lblLoadingUninstaller->hide();

    Utilities::addDropShadow(ui->btnUninstall, 60);
    Utilities::addDropShadow(ui->txtPackageSearch, 40);

    QtConcurrent::run(this, &UninstallerPage::loadPackages);

    connect(SignalMapper::ins(), &SignalMapper::sigUninstallStarted, this, &UninstallerPage::uninstallStarted);
    connect(SignalMapper::ins(), &SignalMapper::sigUninstallFinished, this, &UninstallerPage::loadPackages);
}

void UninstallerPage::loadPackages()
{
    emit uninstallStarted();

    // clear items
    ui->listWidgetPackages->clear();

    QIcon icon(":/static/themes/common/img/package.png");
    QStringList packages = tm->getPackages();
    for (const QString &package : packages) {
        QListWidgetItem *item = new QListWidgetItem(QIcon::fromTheme(package, icon), QString("  %1").arg(package));

        item->setCheckState(Qt::Unchecked);

        ui->listWidgetPackages->addItem(item);
    }
    setAppCount();

    ui->listWidgetPackages->setEnabled(true);
    ui->txtPackageSearch->setEnabled(true);
    ui->txtPackageSearch->clear();

    ui->lblLoadingUninstaller->hide();
}

void UninstallerPage::setAppCount()
{
    int count = ui->listWidgetPackages->count();

    ui->lblPackagesTitle->setText(tr("System Installed Packages (%1)").arg(count));

    ui->notFoundWidget->setVisible(! count);
    ui->btnUninstall->setVisible(count);
    ui->listWidgetPackages->setVisible(count);
}

QStringList UninstallerPage::getSelectedPackages()
{
    QStringList selectedPackages = {};

    for (int i = 0; i < ui->listWidgetPackages->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidgetPackages->item(i);

        if(item->checkState() == Qt::Checked)
            selectedPackages << item->text().trimmed();
    }

    return selectedPackages;
}

void UninstallerPage::on_btnUninstall_clicked()
{
    QStringList selectedPackages = getSelectedPackages();

    if(! selectedPackages.isEmpty()) {
        QtConcurrent::run([=] {
            emit SignalMapper::ins()->sigUninstallStarted();
            ToolManager::ins()->uninstallPackages(selectedPackages);
            emit SignalMapper::ins()->sigUninstallFinished();
        });
    }
}

void UninstallerPage::uninstallStarted()
{
    ui->listWidgetPackages->setEnabled(false);
    ui->txtPackageSearch->setEnabled(false);
    ui->btnUninstall->hide();

    ui->lblLoadingUninstaller->show();
}

void UninstallerPage::on_txtPackageSearch_textChanged(const QString &val)
{
    // Get matches items
    QList<QListWidgetItem*> matches = ui->listWidgetPackages->findItems(val, Qt::MatchFlag::MatchContains);

    // All items hide
    for (int i = 0; i < ui->listWidgetPackages->count(); ++i)
        ui->listWidgetPackages->item(i)->setHidden(true);

    // Matches items show
    for (QListWidgetItem* item : matches)
        item->setHidden(false);
}
