#include "search_page.h"
#include "ui_search_page.h"

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
