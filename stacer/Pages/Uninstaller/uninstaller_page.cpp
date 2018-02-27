#include "uninstaller_page.h"
#include "ui_uninstallerpage.h"
#include <QMovie>

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
    ui->loading->setMovie(loadingMovie);
    loadingMovie->start();
    ui->loading->hide();
    ui->notFoundWidget->hide();

    QtConcurrent::run(this, &UninstallerPage::loadPackages);

    connect(tm, &ToolManager::uninstallStarted, this, &UninstallerPage::uninstallStarted);
    connect(tm, &ToolManager::uninstallFinished, this, &UninstallerPage::loadPackages);
}

void UninstallerPage::loadPackages()
{
    emit uninstallStarted();

    // clear items
    ui->packagesList->clear();

    QIcon icon(QString(":/static/themes/%1/img/package.svg").arg(AppManager::ins()->getThemeName()));
    QStringList packages = tm->getPackages();
    for (const QString &package : packages) {
        QListWidgetItem *item = new QListWidgetItem(QIcon::fromTheme(package, icon), QString("  %1").arg(package));

        item->setCheckState(Qt::Unchecked);

        ui->packagesList->addItem(item);
    }
    setAppCount();

    ui->packagesList->setEnabled(true);
    ui->packageSearch->setEnabled(true);
    ui->packageSearch->clear();

    ui->loading->hide();
}

void UninstallerPage::setAppCount()
{
    int count = ui->packagesList->count();

    ui->packagesTitle->setText(tr("System Installed Packages (%1)").arg(count));

    ui->notFoundWidget->setVisible(! count);
    ui->uninstallBtn->setVisible(count);
    ui->packagesList->setVisible(count);
}

QStringList UninstallerPage::getSelectedPackages()
{
    QStringList selectedPackages = {};

    for (int i = 0; i < ui->packagesList->count(); ++i)
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

    if(! selectedPackages.isEmpty()) {
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
