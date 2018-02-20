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
    fileSystemWatcher.addPath(APT_SOURCE_LIST_PATH);
    connect(&fileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, &APTSourceManagerPage::loadAptSources);

    loadAptSources();
}

void APTSourceManagerPage::loadAptSources()
{
    ui->aptSourceRepositoryListWidget->clear();

    QFileInfoList sourceFileInfoList = ToolManager::ins()->getSourceList();

    for (const QFileInfo &fileInfo: sourceFileInfoList) {

        APTSourceRepositoryItem *aptSourceItem =
                new APTSourceRepositoryItem(fileInfo.fileName(), fileInfo.filePath());

        QListWidgetItem *listItem = new QListWidgetItem(ui->aptSourceRepositoryListWidget);

        listItem->setSizeHint(aptSourceItem->sizeHint());

        ui->aptSourceRepositoryListWidget->setItemWidget(listItem, aptSourceItem);
    }

    ui->notFoundWidget->setVisible(sourceFileInfoList.isEmpty());

    ui->aptSourceTitleLbl->setText(tr("APT Source Repositories (%1)")
                                   .arg(sourceFileInfoList.count()));
}

void APTSourceManagerPage::on_btnAddAPTSourceRepository_clicked()
{
    ui->aptSourceRepositoryListWidget->clear();
}
