#include "search_page.h"
#include "ui_search_page.h"
#include <qdebug.h>
#include <QClipboard>

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
        tr("Name"), tr("Path"), tr("Size"), tr("User"), tr("Group"),
        tr("Creation Time"), tr("Last Access"), tr("Last Modification"), tr("Last Change"),
    };

    // Table settings
    mItemModel->setHorizontalHeaderLabels(mTableHeaders);
    mSortFilterModel->setSourceModel(mItemModel);

    ui->tableFoundResults->setModel(mSortFilterModel);
    mSortFilterModel->setSortRole(1);
    mSortFilterModel->setDynamicSortFilter(true);
    mSortFilterModel->sort(1, Qt::SortOrder::DescendingOrder);

    ui->tableFoundResults->horizontalHeader()->setSectionsMovable(true);
    ui->tableFoundResults->horizontalHeader()->setFixedHeight(32);
    ui->tableFoundResults->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableFoundResults->horizontalHeader()->setCursor(Qt::PointingHandCursor);
    ui->tableFoundResults->horizontalHeader()->resizeSection(0, 150);
    ui->tableFoundResults->horizontalHeader()->resizeSection(1, 150);

    ui->tableFoundResults->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableFoundResults->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableFoundResults->horizontalHeader(), &QHeaderView::customContextMenuRequested,
        this, &SearchPage::tableFoundResults_header_customContextMenuRequested);

    loadHeaderMenu();
    loadTableRowMenu();

    rowRole = 1;
    mSearchResultDateFormat = "dd.MM.yyyy hh:mm:ss";

    ui->advanceSearchPane->setHidden(false);
    on_btnAdvancePaneToggle_clicked();

    ui->lblErrorMsg->hide();

    QString iconLoading = QString(":/static/themes/%1/img/loading.gif").arg(SettingManager::ins()->getThemeName());
    QMovie *loadingMovie = new QMovie(iconLoading, QByteArray(), this);
    ui->lblLoadingSearching->setMovie(loadingMovie);
    loadingMovie->start();
    ui->lblLoadingSearching->hide();

    initComboboxValues();

    QList<QWidget*> widgets = {
        ui->btnBrowseSearchDir, ui->btnSearchAdvance, ui->txtSearchInput, ui->cmbGroups,
        ui->cmbSizeCriteria, ui->cmbSizeUnits, ui->cmbTimeCriteria, ui->cmbTimeType,
         ui->cmbSearchTypes, ui->tableFoundResults, ui->cmbUsers
    };

    Utilities::addDropShadow(widgets, 30);
}

void SearchPage::loadTableRowMenu()
{
    QAction *actionOpenFolder = new QAction(QIcon(":/static/themes/common/img/folder.png"), tr("Open Folder"));
    actionOpenFolder->setData("open-folder");
    mTableRowMenu.addAction(actionOpenFolder);

    QAction *actionMoveTrash = new QAction(QIcon(":/static/themes/common/img/trash_2.png"), tr("Move Trash"));
    actionMoveTrash->setData("move-trash");
    mTableRowMenu.addAction(actionMoveTrash);

    QAction *actionDelete = new QAction(QIcon(":/static/themes/common/img/delete.png"), tr("Delete"));
    actionDelete->setData("delete");
    mTableRowMenu.addAction(actionDelete);
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
    QList<int> hiddenHeaders = { 4, 6, 7, 8 };

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
    QtConcurrent::run(this, &SearchPage::searching);
    ui->advanceSearchPane->hide();
}

void SearchPage::searching()
{
    if (mSelectedDirectory.isEmpty()) {
        ui->lblErrorMsg->show();
        ui->lblErrorMsg->setText(tr("Select the search directory."));
    } else {
        ui->lblErrorMsg->hide();

        ui->lblLoadingSearching->show();
        ui->btnSearchAdvance->setEnabled(false);

        QStringList findQuery(mSelectedDirectory);

        if (! ui->txtSearchInput->text().isEmpty()) {
            if (ui->checkCaseInsensitive->isChecked()) {
                if (ui->checkRegEx->isChecked()) {
                    findQuery.append("-iregex");
                } else {
                    findQuery.append("-iname");
                }
            } else {
                if (ui->checkRegEx->isChecked()) {
                    findQuery.append("-regex");
                } else {
                    findQuery.append("-name");
                }
            }

            findQuery.append(QString("%1").arg(ui->txtSearchInput->text()));
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

        // searching
        QString result;

        try {
            if (ui->checkSearchAsRoot->isChecked()) {
                result = CommandUtil::sudoExec("find", findQuery);
            } else {
                result = CommandUtil::exec("find", findQuery);
            }

            if (result.trimmed().isEmpty()) {
                mItemModel->removeRows(0, mItemModel->rowCount()); // clear table
            } else {
                loadDataToTable(result.split("\n"));
            }
        } catch (QString ex) {
            ui->lblErrorMsg->show();
            ui->lblErrorMsg->setText(tr("Somethings went wrong, try again."));
        }

        ui->lblLoadingSearching->hide();
        ui->btnSearchAdvance->setEnabled(true);
    }
}

void SearchPage::loadDataToTable(const QList<QString> &foundFiles)
{
    mItemModel->removeRows(0, mItemModel->rowCount());

    for (const QString &file : foundFiles.mid(1, 2000)) {
        mItemModel->appendRow(createRow(file));
    }

    ui->lblFoundFilesInfo->setText(tr("%1 files found. Showing %2 of them.")
                                   .arg(foundFiles.count()-1)
                                   .arg(mItemModel->rowCount()));
}

QList<QStandardItem*> SearchPage::createRow(const QString &filepath)
{
    QFileInfo *fileInfo = new QFileInfo(filepath);

    QStandardItem *i_name = new QStandardItem(fileInfo->fileName());
    i_name->setData(fileInfo->fileName(), rowRole);
    i_name->setData(fileInfo->fileName(), Qt::ToolTipRole);

    QStandardItem *i_path = new QStandardItem(fileInfo->path());
    i_path->setData(fileInfo->path(), rowRole);
    i_path->setData(fileInfo->path(), Qt::ToolTipRole);

    QStandardItem *i_size = new QStandardItem(FormatUtil::formatBytes(fileInfo->size()));
    i_size->setData(fileInfo->size(), rowRole);
    i_size->setData(fileInfo->size(), Qt::ToolTipRole);

    QStandardItem *i_user = new QStandardItem(fileInfo->owner());
    i_user->setData(fileInfo->owner(), rowRole);
    i_user->setData(fileInfo->owner(), Qt::ToolTipRole);

    QStandardItem *i_group = new QStandardItem(fileInfo->group());
    i_group->setData(fileInfo->group(), rowRole);
    i_group->setData(fileInfo->group(), Qt::ToolTipRole);

    QStandardItem *i_creationTime = new QStandardItem(fileInfo->created().toString(mSearchResultDateFormat));
    i_creationTime->setData(fileInfo->created().toString(mSearchResultDateFormat), rowRole);
    i_creationTime->setData(fileInfo->created().toString(mSearchResultDateFormat), Qt::ToolTipRole);

    QStandardItem *i_lastAccess = new QStandardItem(fileInfo->lastRead().toString(mSearchResultDateFormat));
    i_lastAccess->setData(fileInfo->lastRead().toString(mSearchResultDateFormat), rowRole);
    i_lastAccess->setData(fileInfo->lastRead().toString(mSearchResultDateFormat), Qt::ToolTipRole);

    QStandardItem *i_lastModify = new QStandardItem(fileInfo->lastModified().toString(mSearchResultDateFormat));
    i_lastModify->setData(fileInfo->lastModified().toString(mSearchResultDateFormat), rowRole);
    i_lastModify->setData(fileInfo->lastModified().toString(mSearchResultDateFormat), Qt::ToolTipRole);

    QStandardItem *i_lastChange = new QStandardItem(fileInfo->metadataChangeTime().toString(mSearchResultDateFormat));
    i_lastChange->setData(fileInfo->metadataChangeTime().toString(mSearchResultDateFormat), rowRole);
    i_lastChange->setData(fileInfo->metadataChangeTime().toString(mSearchResultDateFormat), Qt::ToolTipRole);

    delete fileInfo;

    return {
        i_name, i_path, i_size, i_user, i_group,
        i_creationTime, i_lastAccess, i_lastModify, i_lastChange
    };
}

void SearchPage::tableFoundResults_header_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->tableFoundResults->mapToGlobal(pos);
    QAction *action = mHeaderMenu.exec(globalPos);

    if (action) {
        ui->tableFoundResults->horizontalHeader()->setSectionHidden(action->data().toInt(), ! action->isChecked());
    }
}

void SearchPage::on_tableFoundResults_customContextMenuRequested(const QPoint &pos)
{
    if (mItemModel->rowCount() > 0) {
        QPoint globalPos = ui->tableFoundResults->mapToGlobal(pos);
        QAction *action = mTableRowMenu.exec(globalPos);

        QModelIndexList selecteds = ui->tableFoundResults->selectionModel()->selectedRows();
        QItemSelectionModel *selectionModel = ui->tableFoundResults->selectionModel();

        if (action && ! selecteds.isEmpty()) {
            if (action->data().toString() == "open-folder") {
                for (QModelIndex &index : selecteds) {
                    QUrl folderPath = mSortFilterModel->index(index.row(), 1).data(rowRole).toUrl();
                    QDesktopServices::openUrl(folderPath);
                }
            }
            else if (action->data().toString() == "move-trash") {
                QString trashPath(QDir::homePath() + "/.local/share/Trash");

                while (! selectionModel->selectedRows().isEmpty()) {
                    QModelIndex index = selectionModel->selectedRows().first();

                    QString fileName = mSortFilterModel->index(index.row(), 0).data(rowRole).toString();
                    QString folderPath = mSortFilterModel->index(index.row(), 1).data(rowRole).toString();

                    QString filePath = folderPath + "/" + fileName;

                    bool isAnotherUser = QFileInfo(filePath).owner() != InfoManager::ins()->getUserName();
                    if (isAnotherUser) {
                        CommandUtil::sudoExec("mv", {filePath, trashPath + "/files"});
                    } else {
                        CommandUtil::exec("mv", {filePath, trashPath + "/files"});
                    }

                    if (QFile(filePath).exists()) {
                        selectionModel->select(index, QItemSelectionModel::Deselect);
                    } else {
                        QString infoContent = QString("[Trash Info]\n"
                                            "Path=%1\n"
                                            "DeletionDate=%2")
                                .arg(filePath)
                                .arg(QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss"));

                        FileUtil::writeFile(trashPath + "/info/" + fileName + ".trashinfo", infoContent);

                        mSortFilterModel->removeRow(index.row());
                    }
                }

                selectionModel->clearSelection();
            }
            else if (action->data().toString() == "delete") {
                while (! selectionModel->selectedRows().isEmpty()) {
                    QModelIndex index = selectionModel->selectedRows().first();

                    QString fileName = mSortFilterModel->index(index.row(), 0).data(rowRole).toString();
                    QString folderPath = mSortFilterModel->index(index.row(), 1).data(rowRole).toString();

                    QString filePath = folderPath + "/" + fileName;

                    bool isAnotherUser = QFileInfo(filePath).owner() != InfoManager::ins()->getUserName();
                    if (isAnotherUser) {
                        CommandUtil::sudoExec("rm", {"-rf", filePath });
                    } else {
                        CommandUtil::exec("rm", {"-rf", filePath });
                    }

                    if (QFile(filePath).exists()) {
                        selectionModel->select(index, QItemSelectionModel::Deselect);
                    } else {
                        mSortFilterModel->removeRow(index.row());
                    }
                }

                selectionModel->clearSelection();
            }
        }
    }
}

void SearchPage::on_tableFoundResults_doubleClicked(const QModelIndex &index)
{
    QUrl folderPath = mSortFilterModel->index(index.row(), 1).data(rowRole).toUrl();
    QDesktopServices::openUrl(folderPath);
}
