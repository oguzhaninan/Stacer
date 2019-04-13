#include "search_page.h"
#include "ui_search_page.h"
#include <qdebug.h>

SearchPage::SearchPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchPage)
{
    ui->setupUi(this);

    init();
}

SearchPage::~SearchPage()
{
    delete ui;
}

void SearchPage::init()
{
    ui->advanceSearchPane->setHidden(false);
    on_btnAdvancePaneToggle_clicked();

    on_checkFilterTime_clicked(false);
    on_checkFilterPermissions_clicked(false);
    on_checkFilterSize_clicked(false);
    on_checkFilterOwner_clicked(false);

    ui->cmbUsers->addItems(InfoManager::ins()->getUserList());
    ui->cmbGroups->addItems(InfoManager::ins()->getGroupList());
}

void SearchPage::on_btnBrowseSearchDir_clicked()
{
    QString selectedDirPath = QFileDialog::getExistingDirectory(this, tr("Select Directory"), "/",
                                      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QDir selectedDir(selectedDirPath);

    if (! selectedDirPath.isEmpty() && selectedDir.exists()) {
        ui->lblSearchDir->setText(tr("Directory: %1").arg(selectedDirPath));
    }
}

void SearchPage::on_btnAdvancePaneToggle_clicked()
{
    ui->advanceSearchPane->setHidden(! ui->advanceSearchPane->isHidden());
    QString icon = ui->advanceSearchPane->isHidden() ? "▼" : "▲";
    ui->btnAdvancePaneToggle->setText(tr("Advanced Search %1").arg(icon));
}

void SearchPage::on_checkFilterTime_clicked(bool checked)
{
    ui->cmbTimeUnits->setEnabled(checked);
    ui->cmbTimeCriteria->setEnabled(checked);
    ui->cmbTimeType->setEnabled(checked);
    ui->spinTime->setEnabled(checked);
}

void SearchPage::on_checkFilterPermissions_clicked(bool checked)
{
    ui->checkPermRead->setEnabled(checked);
    ui->checkPermWrite->setEnabled(checked);
    ui->checkPermExec->setEnabled(checked);
}

void SearchPage::on_checkFilterSize_clicked(bool checked)
{
    ui->cmbSizeCriteria->setEnabled(checked);
    ui->cmbSizeUnits->setEnabled(checked);
    ui->spinSize->setEnabled(checked);
}

void SearchPage::on_checkFilterOwner_clicked(bool checked)
{
    ui->cmbUsers->setEnabled(checked);
    ui->cmbGroups->setEnabled(checked);
}
