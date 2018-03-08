#include "apt_source_manager_page.h"
#include "ui_apt_source_manager_page.h"
#include <QDebug>
#include "utilities.h"
#include "Managers/tool_manager.h"

APTSourceManagerPage::~APTSourceManagerPage()
{
    delete ui;
}

APTSourcePtr APTSourceManagerPage::selectedAptSource = nullptr;

APTSourceManagerPage::APTSourceManagerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::APTSourceManagerPage)
{
    ui->setupUi(this);

    init();
}

void APTSourceManagerPage::init()
{
    ui->txtAptSource->setPlaceholderText(tr("example %1")
                                         .arg("'deb http://archive.ubuntu.com/ubuntu xenial main'"));

    loadAptSources();

    on_btnCancel_clicked();

    Utilities::addDropShadow(ui->txtSearchAptSource, 40);
}

void APTSourceManagerPage::loadAptSources()
{
    ui->listWidgetAptSources->clear();

    QList<APTSourcePtr> aptSourceList = ToolManager::ins()->getSourceList();

    for (APTSourcePtr &aptSource: aptSourceList) {

        QListWidgetItem *listItem = new QListWidgetItem(ui->listWidgetAptSources);
        listItem->setData(5, aptSource->source); // for search

        APTSourceRepositoryItem *aptSourceItem = new APTSourceRepositoryItem(aptSource, ui->listWidgetAptSources);

        listItem->setSizeHint(aptSourceItem->sizeHint() + QSize(0, 1));

        ui->listWidgetAptSources->setItemWidget(listItem, aptSourceItem);
    }

    ui->notFoundWidget->setVisible(aptSourceList.isEmpty());

    ui->lblAptSourceTitle->setText(tr("APT Source Repositories (%1)")
                                   .arg(aptSourceList.count()));
}

void APTSourceManagerPage::on_btnAddAPTSourceRepository_clicked(bool checked)
{
    if (checked) {
        ui->btnAddAPTSourceRepository->setText(tr("Save"));
        changeElementsVisible(checked);
    } else {
        QString aptSourceRepository = ui->txtAptSource->text().trimmed();

        if (! aptSourceRepository.isEmpty()) {
            ToolManager::ins()->addAPTRepository(aptSourceRepository, ui->checkEnableSource->isChecked());

            ui->txtAptSource->clear();
            ui->checkEnableSource->setChecked(false);
            on_btnCancel_clicked();
            loadAptSources();
        }
    }
}

void APTSourceManagerPage::on_btnCancel_clicked()
{
    ui->btnAddAPTSourceRepository->setChecked(false);
    changeElementsVisible(false);
    ui->btnAddAPTSourceRepository->setText(tr("Add APT Repository"));
}

void APTSourceManagerPage::changeElementsVisible(const bool checked)
{
    ui->txtAptSource->setVisible(checked);
    ui->checkEnableSource->setVisible(checked);
    ui->btnCancel->setVisible(checked);
    ui->btnEditAptSource->setVisible(!checked);
    ui->btnDeleteAptSource->setVisible(!checked);
    ui->bottomSectionHorizontalSpacer->changeSize(0, 0, checked ? QSizePolicy::Minimum : QSizePolicy::Expanding);
}

void APTSourceManagerPage::on_listWidgetAptSources_itemClicked(QListWidgetItem *item)
{
    QWidget *widget = ui->listWidgetAptSources->itemWidget(item);
    if (widget) {
        APTSourceRepositoryItem *aptSourceItem = dynamic_cast<APTSourceRepositoryItem*>(widget);
        if (aptSourceItem) {
            selectedAptSource = aptSourceItem->aptSource();
        }
    } else {
        selectedAptSource.clear();
    }
}

void APTSourceManagerPage::on_listWidgetAptSources_itemDoubleClicked(QListWidgetItem *item)
{
    on_listWidgetAptSources_itemClicked(item);
    on_btnEditAptSource_clicked();
}

void APTSourceManagerPage::on_btnDeleteAptSource_clicked()
{
    if (! selectedAptSource.isNull()) {
        ToolManager::ins()->removeAPTSource(selectedAptSource);
        loadAptSources();
    }
}

void APTSourceManagerPage::on_txtSearchAptSource_textChanged(const QString &val)
{
    for (int i = 0; i < ui->listWidgetAptSources->count(); ++i) {
        QListWidgetItem *item = ui->listWidgetAptSources->item(i);
        if (item) {
            bool isContain = item->data(5).toString().contains(val, Qt::CaseInsensitive);
            ui->listWidgetAptSources->setItemHidden(item, ! isContain);
        }
    }
}

void APTSourceManagerPage::on_btnEditAptSource_clicked()
{
    if (! selectedAptSource.isNull()) {
        if (mAptSourceEditDialog.isNull()) {
            mAptSourceEditDialog = QSharedPointer<APTSourceEdit>(new APTSourceEdit(this));
            connect(mAptSourceEditDialog.data(), &APTSourceEdit::saved, this, &APTSourceManagerPage::loadAptSources);
        }
        APTSourceEdit::selectedAptSource = selectedAptSource;
        mAptSourceEditDialog->show();
    }
}
