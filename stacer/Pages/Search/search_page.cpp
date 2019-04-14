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

    initComboboxValues();

    QList<QWidget*> widgets = {
        ui->btnBrowseSearchDir, ui->btnSearchAdvance, ui->txtSearchInput, ui->cmbGroups, ui->cmbSearchTypes,
        ui->cmbSizeCriteria, ui->cmbSizeUnits, ui->cmbTimeCriteria, ui->cmbTimeType, ui->cmbTimeUnits, ui->cmbUsers
    };

    Utilities::addDropShadow(widgets, 30);
}

void SearchPage::initComboboxValues()
{
    ui->cmbUsers->addItem(tr("Choose"), "-1");
    ui->cmbUsers->addItems(InfoManager::ins()->getUserList());

    ui->cmbGroups->addItem(tr("Choose"), "-1");
    ui->cmbGroups->addItems(InfoManager::ins()->getGroupList());

    ui->cmbSearchTypes->addItem(tr("All"), "all");
    ui->cmbSearchTypes->addItem(tr("File"), "f");
    ui->cmbSearchTypes->addItem(tr("Directory"), "d");
    ui->cmbSearchTypes->addItem(tr("Symbolic Link"), "l");

    ui->cmbTimeType->addItem(tr("Access"), "atime");
    ui->cmbTimeType->addItem(tr("Modify"), "mtime");
    ui->cmbTimeType->addItem(tr("Change"), "ctime");

    ui->cmbTimeCriteria->addItem(tr("Smaller (<)"), "-");
    ui->cmbTimeCriteria->addItem(tr("Equal (=)"), "");
    ui->cmbTimeCriteria->addItem(tr("Greater (<)"), "+");

    ui->cmbTimeUnits->addItem(tr("Second"), "s");
    ui->cmbTimeUnits->addItem(tr("Second"), "s");
    ui->cmbTimeUnits->addItem(tr("Second"), "s");
    ui->cmbTimeUnits->addItem(tr("Second"), "s");
    ui->cmbTimeUnits->addItem(tr("Second"), "s");

    ui->cmbSizeCriteria->addItem(tr("Choose"), "-1");
    ui->cmbSizeCriteria->addItem(tr("Smaller (<)"), "-");
    ui->cmbSizeCriteria->addItem(tr("Equal (=)"), "");
    ui->cmbSizeCriteria->addItem(tr("Greater (<)"), "+");

    ui->cmbSizeUnits->addItem("Bytes", "c");
    ui->cmbSizeUnits->addItem("Kibibytes", "k");
    ui->cmbSizeUnits->addItem("Mebibytes", "M");
    ui->cmbSizeUnits->addItem("Bytes", "G");
}

void SearchPage::on_btnBrowseSearchDir_clicked()
{
    QString selectedDirPath = QFileDialog::getExistingDirectory(this, tr("Select Directory"), "/",
                                      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QDir selectedDir(selectedDirPath);

    if (! selectedDirPath.isEmpty() && selectedDir.exists()) {
        ui->lblSearchDir->setText(tr("Directory: %1").arg(selectedDirPath));
        mSelectedDirectory = selectedDirPath;
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

void SearchPage::on_btnSearchAdvance_clicked()
{
    if (mSelectedDirectory.isEmpty()) {


    } else {
        QStringList findQuery(mSelectedDirectory);

        if (! ui->txtSearchInput->text().isEmpty()) {
            if (ui->checkRegEx->isChecked()) {
                findQuery.append("-iname");
            } else {
                findQuery.append("-name");
            }

            findQuery.append(QString("'%1'").arg(ui->txtSearchInput->text()));
        }

        if (ui->checkInvert->isChecked()) {
            findQuery.append("-invert");
        }

        if (ui->checkEmpty->isChecked()) {
            findQuery.append("-empty");
        }

        // TIME
        if (ui->checkFilterTime->isChecked()) {

        }

        // PERMISSIONS
        if (ui->checkPermReadable->isChecked()) {
            findQuery.append("-readable");
        }

        if (ui->checkPermWritable->isChecked()) {
            findQuery.append("-writable");
        }

        if (ui->checkPermExecutable->isChecked()) {
            findQuery.append("-executable");
        }

        // SIZE
        if (ui->cmbSizeCriteria->currentData().toString() != "-1") {
            QString size = QString("%1%2%3")
                    .arg(ui->cmbSizeCriteria->currentData().toString())
                    .arg(ui->spinSize->value())
                    .arg(ui->cmbSizeUnits->currentData().toString());

            findQuery.append("-size");
            findQuery.append(size);
        }

        // OWNER
        if (ui->cmbUsers->currentData().toString() != "-1") {
            findQuery.append("-user");
            findQuery.append(ui->cmbUsers->currentText());
        }

        if (ui->cmbGroups->currentData().toString() != "-1") {
            findQuery.append("-group");
            findQuery.append(ui->cmbGroups->currentText());
        }

        // searching
        QString result;

        try {
            if (ui->checkSearchAsRoot->isChecked()) {
                //result = CommandUtil::sudoExec("find", findQuery);
            } else {
                //result = CommandUtil::exec("find", findQuery);
            }
        } catch (QString ex) {

        }

        qDebug() << findQuery;

        qDebug() << result.split("\n");
    }
}
