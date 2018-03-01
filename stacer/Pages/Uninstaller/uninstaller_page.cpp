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
    QString path = QString(":/static/themes/%1/img/loading.gif").arg(AppManager::ins()->getThemeName());
    QMovie *loadingMovie = new QMovie(path, QByteArray(), this);
    ui->lblLoadingUninstaller->setMovie(loadingMovie);
    loadingMovie->start();
    ui->lblLoadingUninstaller->hide();
    ui->notFoundWidget->hide();

    Utilities::addDropShadow(ui->btnUninstall, 60);
    Utilities::addDropShadow(ui->txtPackageSearch, 40);

    QtConcurrent::run(this, &UninstallerPage::loadPackages);

    connect(tm, &ToolManager::uninstallStarted, this, &UninstallerPage::uninstallStarted);
    connect(tm, &ToolManager::uninstallFinished, this, &UninstallerPage::loadPackages);
}

void UninstallerPage::loadPackages()
{
    emit uninstallStarted();

    // clear items
    ui->listWidgetPackages->clear();

    QIcon icon(QString(":/static/themes/%1/img/package.svg").arg(AppManager::ins()->getThemeName()));
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
        QtConcurrent::run([selectedPackages]() -> void {
            ToolManager::ins()->uninstallPackages(selectedPackages);
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
