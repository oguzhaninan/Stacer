#include "search_page.h"
#include "ui_search_page.h"
#include <qdebug.h>

SearchPage::SearchPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchPage),
    mItemModel(new QStandardItemModel(this)),
    mSortFilterModel(new QSortFilterProxyModel(this))
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
    mTableHeaders = QStringList {
        tr("Name"), tr("Path"), tr("Size"), tr("User"), tr("Group"), tr("Permissions"),
        tr("Creation Time"), tr("Last Access"), tr("Last Modification"), tr("Last Change"),
    };

    // Table settings
    mSortFilterModel->setSourceModel(mItemModel);

    mItemModel->setHorizontalHeaderLabels(mTableHeaders);

    ui->tableFoundResults->setModel(mSortFilterModel);
    mSortFilterModel->setSortRole(1);
    mSortFilterModel->setDynamicSortFilter(true);
    mSortFilterModel->sort(1, Qt::SortOrder::DescendingOrder);

    ui->tableFoundResults->horizontalHeader()->setSectionsMovable(true);
    ui->tableFoundResults->horizontalHeader()->setFixedHeight(32);
    ui->tableFoundResults->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableFoundResults->horizontalHeader()->setCursor(Qt::PointingHandCursor);
    ui->tableFoundResults->horizontalHeader()->resizeSection(0, 130);
    ui->tableFoundResults->horizontalHeader()->resizeSection(1, 130);

    ui->tableFoundResults->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableFoundResults->horizontalHeader(), SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(on_tableFoundResults_customContextMenuRequested(const QPoint&)));

    loadHeaderMenu();

    ui->advanceSearchPane->setHidden(false);
    on_btnAdvancePaneToggle_clicked();

    initComboboxValues();

    QList<QWidget*> widgets = {
        ui->btnBrowseSearchDir, ui->btnSearchAdvance, ui->txtSearchInput, ui->cmbGroups, ui->cmbSearchTypes,
        ui->cmbSizeCriteria, ui->cmbSizeUnits, ui->cmbTimeCriteria, ui->cmbTimeType, ui->cmbUsers,
        ui->tableFoundResults
    };

    Utilities::addDropShadow(widgets, 30);
}

void SearchPage::loadHeaderMenu()
{
    int i = 0;
    for (const QString &header : mTableHeaders) {
        QAction *action = new QAction(header);
        action->setCheckable(true);
        action->setChecked(true);
        action->setData(i++);

        mHeaderMenu.addAction(action);
    }

    // exclude headers
    QList<int> hiddenHeaders = { 4, 5, 7, 8, 9 };

    QList<QAction*> actions = mHeaderMenu.actions();
    for (const int i : hiddenHeaders) {
        if (i < mTableHeaders.count()) {
            ui->tableFoundResults->horizontalHeader()->setSectionHidden(i, true);
            actions.at(i)->setChecked(false);
        }
    }
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

    ui->cmbTimeType->addItem(tr("Choose"), "-1");
    ui->cmbTimeType->addItem(tr("Access"), "-amin");
    ui->cmbTimeType->addItem(tr("Modify"), "-mmin");
    ui->cmbTimeType->addItem(tr("Change"), "-cmin");

    ui->cmbTimeCriteria->addItem(tr("Smaller (<)"), "-");
    ui->cmbTimeCriteria->addItem(tr("Equal (=)"), "");
    ui->cmbTimeCriteria->addItem(tr("Greater (>)"), "+");

    ui->cmbSizeCriteria->addItem(tr("Choose"), "-1");
    ui->cmbSizeCriteria->addItem(tr("Smaller (<)"), "-");
    ui->cmbSizeCriteria->addItem(tr("Equal (=)"), "");
    ui->cmbSizeCriteria->addItem(tr("Greater (>)"), "+");

    ui->cmbSizeUnits->addItem("Bytes", "c");
    ui->cmbSizeUnits->addItem("Kibibytes", "k");
    ui->cmbSizeUnits->addItem("Mebibytes", "M");
    ui->cmbSizeUnits->addItem("Gibibytes", "G");
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

        if (ui->cmbSearchTypes->currentData().toString() != "all") {
            findQuery.append("-type");
            findQuery.append(ui->cmbSearchTypes->currentData().toString());
        }

        // TIME
        if (ui->cmbTimeType->currentData().toString() != "-1") {
            findQuery.append(ui->cmbTimeType->currentData().toString());
            findQuery.append(QString("%1%2").arg(ui->cmbTimeCriteria->currentData().toString()).arg(ui->spinTime->value()));
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

        //findQuery.append("-printf");
        //findQuery.append("'%y|%p|%f|%s|%A+|%T+|%C+|%u|%g\n'");
        //findQuery.append("'%p'");

        // searching
        QString result;

        try {
            if (ui->checkSearchAsRoot->isChecked()) {
                result = CommandUtil::sudoExec("find", findQuery);
            } else {
                result = CommandUtil::exec("find", findQuery);
            }

            QList<QFileInfo> foundFiles;
            for (const QString &filepath: result.split("\n")) {
                foundFiles << QFileInfo(filepath);
            }

            loadDataToTable(foundFiles);

        } catch (QString ex) {
            qDebug() << ex;
        }

    }
}

void SearchPage::loadDataToTable(const QList<QFileInfo> &foundFiles)
{
    mItemModel->removeRows(0, mItemModel->rowCount());

    for (const QFileInfo &file : foundFiles) {
        mItemModel->appendRow(createRow(file));
    }

    ui->lblFoundFilesInfo->setText(tr("%1 files found").arg(foundFiles.count()));
}

QList<QStandardItem*> SearchPage::createRow(const QFileInfo &fileInfo)
{
    int data = 1;
    QString dateFormat("dd.MM.yyyy hh:mm:ss");

    QStandardItem *i_name = new QStandardItem(fileInfo.fileName());
    i_name->setData(fileInfo.fileName(), data);
    i_name->setData(fileInfo.fileName(), Qt::ToolTipRole);

    QStandardItem *i_path = new QStandardItem(fileInfo.path());
    i_path->setData(fileInfo.path(), data);
    i_path->setData(fileInfo.path(), Qt::ToolTipRole);

    QStandardItem *i_size = new QStandardItem(FormatUtil::formatBytes(fileInfo.size()));
    i_size->setData(fileInfo.size(), data);
    i_size->setData(fileInfo.size(), Qt::ToolTipRole);

    QStandardItem *i_user = new QStandardItem(fileInfo.owner());
    i_user->setData(fileInfo.owner(), data);
    i_user->setData(fileInfo.owner(), Qt::ToolTipRole);

    QStandardItem *i_group = new QStandardItem(fileInfo.group());
    i_group->setData(fileInfo.group(), data);
    i_group->setData(fileInfo.group(), Qt::ToolTipRole);

    QStandardItem *i_permissions = new QStandardItem(fileInfo.group());
    i_permissions->setData(fileInfo.group(), data);
    i_permissions->setData(fileInfo.group(), Qt::ToolTipRole);

    QStandardItem *i_creationTime = new QStandardItem(fileInfo.birthTime().toString(dateFormat));
    i_creationTime->setData(fileInfo.birthTime().toString(dateFormat), data);
    i_creationTime->setData(fileInfo.birthTime().toString(dateFormat), Qt::ToolTipRole);

    QStandardItem *i_lastAccess = new QStandardItem(fileInfo.lastRead().toString(dateFormat));
    i_lastAccess->setData(fileInfo.lastRead().toString(dateFormat), data);
    i_lastAccess->setData(fileInfo.lastRead().toString(dateFormat), Qt::ToolTipRole);

    QStandardItem *i_lastModify = new QStandardItem(fileInfo.lastModified().toString(dateFormat));
    i_lastModify->setData(fileInfo.lastModified().toString(dateFormat), data);
    i_lastModify->setData(fileInfo.lastModified().toString(dateFormat), Qt::ToolTipRole);

    QStandardItem *i_lastChange = new QStandardItem(fileInfo.metadataChangeTime().toString(dateFormat));
    i_lastChange->setData(fileInfo.metadataChangeTime().toString(dateFormat), data);
    i_lastChange->setData(fileInfo.metadataChangeTime().toString(dateFormat), Qt::ToolTipRole);

    return {
        i_name, i_path, i_size, i_user, i_group, i_permissions,
        i_creationTime, i_lastAccess, i_lastModify, i_lastChange
    };
}

void SearchPage::on_tableFoundResults_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->tableFoundResults->mapToGlobal(pos);
    QAction *action = mHeaderMenu.exec(globalPos);

    if (action) {
        ui->tableFoundResults->horizontalHeader()->setSectionHidden(action->data().toInt(), ! action->isChecked());
    }
}
