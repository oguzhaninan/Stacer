#include "apt_source_manager_page.h"
#include "ui_apt_source_manager_page.h"
#include <QDebug>

#include "Managers/tool_manager.h"

APTSourceManagerPage::~APTSourceManagerPage()
{
    delete ui;
}

QString APTSourceManagerPage::selectedAptSource = QString();

APTSourceManagerPage::APTSourceManagerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::APTSourceManagerPage),
    fileSystemWatcher(this)
{
    ui->setupUi(this);

    init();

    ui->aptSourceRepositoryListWidget->setFocus();
}

void APTSourceManagerPage::init()
{
    fileSystemWatcher.addPaths({ APT_SOURCES_LIST_D_PATH, APT_SOURCES_LIST_PATH });
    connect(&fileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, &APTSourceManagerPage::loadAptSources);
    connect(&fileSystemWatcher, &QFileSystemWatcher::fileChanged, this, &APTSourceManagerPage::loadAptSources);

    ui->txtAptSource->setPlaceholderText(tr("example %1").arg("'deb http://archive.ubuntu.com/ubuntu xenial main'"));

    loadAptSources();

    on_btnAddAPTSourceRepository_clicked(false);
}

void APTSourceManagerPage::loadAptSources()
{
    ui->aptSourceRepositoryListWidget->clear();

    QList<APTSourcePtr> aptSourceList = ToolManager::ins()->getSourceList();

    for (APTSourcePtr &aptSource: aptSourceList) {

        QListWidgetItem *listItem = new QListWidgetItem(ui->aptSourceRepositoryListWidget);
        listItem->setData(0, aptSource->source); // for search

        APTSourceRepositoryItem *aptSourceItem = new APTSourceRepositoryItem(aptSource, ui->aptSourceRepositoryListWidget);

        listItem->setSizeHint(aptSourceItem->sizeHint());

        ui->aptSourceRepositoryListWidget->setItemWidget(listItem, aptSourceItem);
    }

    ui->notFoundWidget->setVisible(aptSourceList.isEmpty());

    ui->aptSourceTitleLbl->setText(tr("APT Source Repositories (%1)")
                                   .arg(aptSourceList.count()));
}

void APTSourceManagerPage::on_btnAddAPTSourceRepository_clicked(bool checked)
{
    changeElementsVisible(checked);
    if (checked) {
        ui->btnAddAPTSourceRepository->setText(tr("Save"));
    }
    else {
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
        ui->btnAddAPTSourceRepository->setText(tr("Add APT Source"));
    }
}

void APTSourceManagerPage::changeElementsVisible(const bool checked)
{
    ui->txtAptSource->setVisible(checked);
    ui->checkEnableSource->setVisible(checked);
    ui->btnEditAptSource->setVisible(!checked);
    ui->btnDeleteAptSource->setVisible(!checked);
    if (checked)
        ui->bottomSectionHorizontalSpacer->changeSize(0, 0, QSizePolicy::Minimum);
    else
        ui->bottomSectionHorizontalSpacer->changeSize(0, 0, QSizePolicy::Expanding);
}

void APTSourceManagerPage::on_aptSourceRepositoryListWidget_itemClicked(QListWidgetItem *item)
{
//    QWidget *widget = ui->aptSourceRepositoryListWidget->itemWidget(item);
    if (item) {
//        APTSourceRepositoryItem *aptSourceItem = dynamic_cast<APTSourceRepositoryItem*>(widget);
//        if (aptSourceItem) {
            selectedAptSource = item->data(0).toString();
//        }
    } else {
        selectedAptSource.clear();
    }
}

void APTSourceManagerPage::on_btnDeleteAptSource_clicked()
{
    if (! selectedAptSource.isNull()) {
        ToolManager::ins()->removeAPTSource(selectedAptSource);
    }
}

void APTSourceManagerPage::on_txtSearchAptSource_textChanged(const QString &val)
{
    if (! val.isEmpty()) {
        for (int i = 0; i < ui->aptSourceRepositoryListWidget->count(); ++i) {
            QListWidgetItem *item = ui->aptSourceRepositoryListWidget->item(i);
            if (item) {
                bool isContain = item->data(0).toString().contains(val, Qt::CaseInsensitive);
                ui->aptSourceRepositoryListWidget->setItemHidden(item, ! isContain);
            }
        }
    }
}
