#include "apt_source_manager_page.h"
#include "ui_apt_source_manager_page.h"
#include <QDebug>

#include "Managers/tool_manager.h"

APTSourceManagerPage::~APTSourceManagerPage()
{
    delete ui;
}

APTSourceManagerPage::APTSourceManagerPage(QWidget *parent) :
    QWidget(parent),
    fileSystemWatcher(this),
    ui(new Ui::APTSourceManagerPage)
{
    ui->setupUi(this);

    init();
}

void APTSourceManagerPage::init()
{
    fileSystemWatcher.addPath(APT_SOURCES_LIST_D_PATH);
    connect(&fileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, &APTSourceManagerPage::loadAptSources);

    ui->txtAptSource->setPlaceholderText(tr("example %1").arg("'deb http://archive.ubuntu.com/ubuntu xenial main'"));

    loadAptSources();
}

void APTSourceManagerPage::loadAptSources()
{
    ui->aptSourceRepositoryListWidget->clear();

    QList<APTSourcePtr> aptSourceList = ToolManager::ins()->getSourceList();

    for (APTSourcePtr &aptSource: aptSourceList) {

        QListWidgetItem *listItem = new QListWidgetItem(ui->aptSourceRepositoryListWidget);

        APTSourceRepositoryItem *aptSourceItem = new APTSourceRepositoryItem(aptSource, ui->aptSourceRepositoryListWidget);

        listItem->setSizeHint(aptSourceItem->sizeHint());

        ui->aptSourceRepositoryListWidget->setItemWidget(listItem, aptSourceItem);
    }

    ui->notFoundWidget->setVisible(aptSourceList.isEmpty());

    ui->aptSourceTitleLbl->setText(tr("APT Source Repositories (%1)")
                                   .arg(aptSourceList.count()));
}

void APTSourceManagerPage::on_btnAddAPTSourceRepository_clicked()
{
    QString aptSourceRepository = ui->txtAptSource->text().trimmed();

    if (!aptSourceRepository.isEmpty()) {
        QStringList args = { "-y", aptSourceRepository };
        if (ui->checkEnableSource->isChecked()) {
            args << "-s";
        }

        CommandUtil::sudoExec("add-apt-repository", args);

        ui->txtAptSource->clear();
        ui->checkEnableSource->setChecked(false);
    }
}
