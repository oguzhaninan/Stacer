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

    ui->stackedWidget->setCurrentIndex(0);

    QList<QWidget*> widgets = { ui->txtPackageSearch, ui->btnUninstall, ui->btnSystemPackages, ui->btnSnapPackages };
    Utilities::addDropShadow(widgets, 40);

    QtConcurrent::run(this, &UninstallerPage::loadPackages);
    QtConcurrent::run(this, &UninstallerPage::loadSnapPackages);

    connect(SignalMapper::ins(), &SignalMapper::sigUninstallStarted, this, &UninstallerPage::uninstallStarted);
    connect(SignalMapper::ins(), &SignalMapper::sigUninstallFinished, this, &UninstallerPage::loadPackages);
    connect(SignalMapper::ins(), &SignalMapper::sigUninstallFinished, this, &UninstallerPage::loadSnapPackages);
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

void UninstallerPage::loadSnapPackages()
{
    // clear items
    ui->listWidgetSnapPackages->clear();

    QIcon icon(":/static/themes/common/img/package.png");
    QStringList packages = tm->getSnapPackages();
    for (const QString &package : packages) {
        QListWidgetItem *item = new QListWidgetItem(QIcon::fromTheme(package, icon), QString("  %1").arg(package));

        item->setCheckState(Qt::Unchecked);

        ui->listWidgetSnapPackages->addItem(item);
    }
    setAppCount();

    ui->listWidgetSnapPackages->setEnabled(true);
    ui->txtPackageSearch->setEnabled(true);
    ui->txtPackageSearch->clear();

    ui->lblLoadingUninstaller->hide();
}

void UninstallerPage::setAppCount()
{
    int count = ui->listWidgetPackages->count();
    ui->btnSystemPackages->setText(tr("Packages (%1)").arg(count));
    ui->notFoundWidget->setVisible(! count);
    ui->listWidgetPackages->setVisible(count);

    int snapCount = ui->listWidgetSnapPackages->count();
    ui->btnSnapPackages->setText(tr("Snap Packages (%1)").arg(snapCount));
    ui->notFoundWidget_2->setVisible(! snapCount);
    ui->listWidgetSnapPackages->setVisible(snapCount);

    ui->btnSnapPackages->setVisible(CommandUtil::isExecutable("snap"));

    ui->btnUninstall->setVisible(count || snapCount);
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

QStringList UninstallerPage::getSelectedSnapPackages()
{
    QStringList selectedPackages = {};

    for (int i = 0; i < ui->listWidgetSnapPackages->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidgetSnapPackages->item(i);

        if(item->checkState() == Qt::Checked)
            selectedPackages << item->text().trimmed();
    }

    return selectedPackages;
}

void UninstallerPage::on_btnUninstall_clicked()
{
    QStringList selectedPackages = getSelectedPackages();
    QStringList selectedSnapPackages = getSelectedSnapPackages();

    if (!selectedPackages.isEmpty() || !selectedSnapPackages.isEmpty()) {
        QtConcurrent::run([=]
        {
            emit SignalMapper::ins()->sigUninstallStarted();

            ToolManager::ins()->uninstallPackages(selectedPackages);
            ToolManager::ins()->uninstallSnapPackages(selectedSnapPackages);

            emit SignalMapper::ins()->sigUninstallFinished();
        });
    }
}

void UninstallerPage::uninstallStarted()
{
    ui->listWidgetPackages->setEnabled(false);
    ui->listWidgetSnapPackages->setEnabled(false);
    ui->txtPackageSearch->setEnabled(false);
    ui->btnUninstall->hide();
    ui->lblLoadingUninstaller->show();
}

void UninstallerPage::on_txtPackageSearch_textChanged(const QString &val)
{
    QListWidget *listWidgetPackages = nullptr;

    switch (ui->stackedWidget->currentIndex()) {
        case 0: listWidgetPackages = ui->listWidgetPackages; break;
        case 1: listWidgetPackages = ui->listWidgetSnapPackages; break;
    }

    // Get matches items
    QList<QListWidgetItem*> matches = listWidgetPackages->findItems(val, Qt::MatchFlag::MatchContains);

    // All items hide
    for (int i = 0; i < listWidgetPackages->count(); ++i)
        listWidgetPackages->item(i)->setHidden(true);

    // Matches items show
    for (QListWidgetItem* item : matches)
        item->setHidden(false);
}

void UninstallerPage::on_btnSystemPackages_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void UninstallerPage::on_btnSnapPackages_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void UninstallerPage::on_listWidgetSnapPackages_itemClicked(QListWidgetItem *item)
{
    //item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    ui->btnUninstall->setText(tr("Uninstall Selected (%1)")
                              .arg(getSelectedSnapPackages().count() + getSelectedPackages().count()));
}

void UninstallerPage::on_listWidgetPackages_itemClicked(QListWidgetItem *item)
{
    //item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    ui->btnUninstall->setText(tr("Uninstall Selected (%1)")
                              .arg(getSelectedSnapPackages().count() + getSelectedPackages().count()));
}
