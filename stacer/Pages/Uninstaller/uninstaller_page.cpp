#include "uninstaller_page.h"
#include "ui_uninstallerpage.h"

UninstallerPage::~UninstallerPage()
{
    delete ui;
    delete loadingMovie;
}

UninstallerPage::UninstallerPage(QWidget *parent) :
    QWidget(parent),
    tm(ToolManager::ins()),
    icon(QIcon(QString(":/static/themes/%1/img/package.svg").arg(AppManager::ins()->getThemeName()))),
    loadingMovie(new QMovie(QString(":/static/themes/%1/img/loading.gif").arg(AppManager::ins()->getThemeName()))),
    ui(new Ui::UninstallerPage)
{
    ui->setupUi(this);

    init();
}

void UninstallerPage::init()
{
    ui->loading->setMovie(loadingMovie);
    loadingMovie->start();
    ui->loading->hide();
    ui->notFoundWidget->hide();

    QtConcurrent::run(this, &UninstallerPage::loadPackages);

    connect(tm, &ToolManager::uninstallFinished, this, &UninstallerPage::loadPackages);
    connect(tm, &ToolManager::uninstallStarted, this, &UninstallerPage::uninstallStarted);
}

void UninstallerPage::loadPackages()
{
    uninstallStarted();

    // clear items
    ui->packagesList->clear();

    for (const QString &package : tm->getPackages())
    {
        QListWidgetItem *item = new QListWidgetItem(QIcon::fromTheme(package, icon), QString("  %1").arg(package));

        item->setCheckState(Qt::Unchecked);

        ui->packagesList->addItem(item);
    }
    setAppCount();

    ui->packagesList->setEnabled(true);
    ui->packageSearch->setEnabled(true);
    ui->uninstallBtn->show();
    ui->packageSearch->clear();

    ui->loading->hide();
}

void UninstallerPage::setAppCount()
{
    int count = ui->packagesList->count();

    ui->packagesTitle->setText(
        tr("System Installed Packages (%1)")
        .arg(QString::number(count)));

    ui->notFoundWidget->setVisible(! count);
    ui->packagesList->setVisible(count);
}

QStringList UninstallerPage::getSelectedPackages()
{
    QStringList selectedPackages = {};

    for (int i = 0; i < ui->packagesList->count(); i++)
    {
        QListWidgetItem *item = ui->packagesList->item(i);

        if(item->checkState() == Qt::Checked)
            selectedPackages << item->text().trimmed();
    }

    return selectedPackages;
}

void UninstallerPage::on_uninstallBtn_clicked()
{
    QStringList selectedPackages = getSelectedPackages();

    if(! selectedPackages.isEmpty())
    {
        QtConcurrent::run([selectedPackages]() -> void {
            ToolManager::ins()->uninstallPackages(selectedPackages);
        });
    }
}

void UninstallerPage::uninstallStarted()
{
    ui->packagesList->setEnabled(false);
    ui->packageSearch->setEnabled(false);
    ui->uninstallBtn->hide();

    ui->loading->show();
}

void UninstallerPage::on_packageSearch_textChanged(const QString &val)
{
    // Get matches items
    QList<QListWidgetItem*> matches = ui->packagesList->findItems(val, Qt::MatchFlag::MatchContains);

    // All items hide
    for (int i = 0; i < ui->packagesList->count(); ++i)
        ui->packagesList->item(i)->setHidden(true);

    // Matches items show
    for (QListWidgetItem* item : matches)
        item->setHidden(false);
}
