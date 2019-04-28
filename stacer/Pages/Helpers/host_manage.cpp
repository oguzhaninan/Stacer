#include "host_manage.h"
#include "ui_host_manage.h"
#include <qdebug.h>

HostManage::~HostManage()
{
    delete ui;
}

HostManage::HostManage(QWidget *parent):
    QWidget(parent),
    mItemModel(new QStandardItemModel(this)),
    mSortFilterModel(new QSortFilterProxyModel(this)),
    updatedLine(-1),
    ui(new Ui::HostManage)
{
    ui->setupUi(this);

    init();
}

void HostManage::init()
{
    ui->lblHostTitle->setText(tr("Hosts (%1)").arg(1));

    Utilities::addDropShadow({
        ui->btnCancel, ui->btnNewHost, ui->btnSave, ui->txtAliases, ui->txtFullyQualified,
        ui->txtIP, ui->tableViewHosts
    }, 40);

    ui->widgetAddEditHost->hide();
    ui->lblErrorMsg->hide();

    mHeaderList = {
       tr("IP Address"), tr("Full Qualified"), tr("Aliases")
    };

    mItemModel->setHorizontalHeaderLabels(mHeaderList);
    mSortFilterModel->setSourceModel(mItemModel);

    ui->tableViewHosts->setModel(mSortFilterModel);
    mSortFilterModel->setSortRole(1);
    mSortFilterModel->setDynamicSortFilter(true);

    ui->tableViewHosts->horizontalHeader()->setSectionsMovable(true);
    ui->tableViewHosts->horizontalHeader()->setFixedHeight(32);
    ui->tableViewHosts->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableViewHosts->horizontalHeader()->setCursor(Qt::PointingHandCursor);
    ui->tableViewHosts->horizontalHeader()->resizeSection(0, 195);
    ui->tableViewHosts->horizontalHeader()->resizeSection(1, 195);

    ui->tableViewHosts->setContextMenuPolicy(Qt::CustomContextMenu);

    loadTableRowMenu();

    mHostFileContent = FileUtil::readListFromFile("/etc/hosts");

    loadTableData();
}

void HostManage::loadHostItems()
{
    mHostItemList.clear();

    int i = 0;
    for (const QString &line: mHostFileContent)
    {
        if (! line.trimmed().startsWith("#") && ! line.trimmed().isEmpty())
        {
            QStringList lineItems = line.trimmed().split(QRegExp("\\s+"));

            if (lineItems.count() > 1) {
                HostItem hItem;
                hItem.ip = lineItems.at(0).trimmed();
                hItem.fullQualified = lineItems.at(1).trimmed();
                hItem.aliases = lineItems.count() > 2 ? lineItems.mid(2).join(" ") : "";

                mHostItemList.insert(i, hItem);
            }
        }
        i++;
    }
}

void HostManage::loadTableData()
{
    loadHostItems();

    mItemModel->removeRows(0, mItemModel->rowCount());

    QMapIterator<int,HostItem> itemIterator(mHostItemList);

    while (itemIterator.hasNext()) {
        itemIterator.next();
        mItemModel->appendRow(createRow(QPair<int, HostItem>(itemIterator.key(), itemIterator.value())));
    }

    ui->lblHostTitle->setText(tr("Hosts (%1)").arg(mHostItemList.count()));
}

QList<QStandardItem*> HostManage::createRow(const QPair<int, HostItem> &item)
{
    QStandardItem *i_ip = new QStandardItem(item.second.ip);
    i_ip->setData(item.first, 9);
    i_ip->setData(item.second.ip, 1);
    i_ip->setData(item.second.ip, Qt::ToolTipRole);

    QStandardItem *i_fullQualified = new QStandardItem(item.second.fullQualified);
    i_fullQualified->setData(item.second.fullQualified, 1);
    i_fullQualified->setData(item.second.fullQualified, Qt::ToolTipRole);

    QStandardItem *i_aliases = new QStandardItem(item.second.aliases);
    i_aliases->setData(item.second.aliases, 1);
    i_aliases->setData(item.second.aliases, Qt::ToolTipRole);

    return {
        i_ip, i_fullQualified, i_aliases
    };
}

void HostManage::on_btnNewHost_clicked()
{
    ui->widgetAddEditHost->show();
    ui->lblErrorMsg->hide();

    ui->txtIP->clear();
    ui->txtFullyQualified->clear();
    ui->txtAliases->clear();

    updatedLine = -1;
}

void HostManage::loadTableRowMenu()
{
    QAction *actionOpenFolder = new QAction(QIcon(":/static/themes/common/img/folder.png"), tr("Edit"));
    actionOpenFolder->setData("edit");
    mTableRowMenu.addAction(actionOpenFolder);

    QAction *actionDelete = new QAction(QIcon(":/static/themes/common/img/delete.png"), tr("Delete"));
    actionDelete->setData("delete");
    mTableRowMenu.addAction(actionDelete);
}

void HostManage::on_btnSave_clicked()
{
    if (ui->txtIP->text().isEmpty() || ui->txtFullyQualified->text().isEmpty()) {
        ui->lblErrorMsg->setText(tr("The IP and Fully Qualified fields are required."));
        ui->lblErrorMsg->show();
    }
    else {
        QString item = QString("%1 %2 %3")
                .arg(ui->txtIP->text().trimmed())
                .arg(ui->txtFullyQualified->text().trimmed())
                .arg(ui->txtAliases->text());

        if (updatedLine == -1) {
            mHostFileContent.append(item);
        } else {
            mHostFileContent.replace(updatedLine, item);
        }

        updatedLine = -1;

        loadTableData();

        ui->widgetAddEditHost->hide();
    }
}

void HostManage::on_btnCancel_clicked()
{
    ui->widgetAddEditHost->hide();
    ui->lblErrorMsg->hide();

    updatedLine = -1;
}

void HostManage::on_btnSaveChanges_clicked()
{
    FileUtil::writeFile("/tmp/stacer_etc_host_new_content", mHostFileContent.join("\n"));
    try {
        CommandUtil::sudoExec("mv", {"/tmp/stacer_etc_host_new_content", "/etc/hosts"});
        loadTableData();
    } catch (QString ex) {
        qDebug() << ex;
    }
}

void HostManage::on_tableViewHosts_customContextMenuRequested(const QPoint &pos)
{
    if (mItemModel->rowCount() > 0) {
        QPoint globalPos = ui->tableViewHosts->mapToGlobal(pos);
        QAction *action = mTableRowMenu.exec(globalPos);

        QModelIndexList selecteds = ui->tableViewHosts->selectionModel()->selectedRows();
        QItemSelectionModel *selectionModel = ui->tableViewHosts->selectionModel();

        if (action && ! selecteds.isEmpty()) {
            if (action->data().toString() == "edit") {
                QModelIndex index = selectionModel->selectedRows().first();

                updatedLine = mSortFilterModel->index(index.row(), 0).data(9).toInt();

                ui->txtIP->setText(mHostItemList.value(updatedLine).ip);
                ui->txtFullyQualified->setText(mHostItemList.value(updatedLine).fullQualified);
                ui->txtAliases->setText(mHostItemList.value(updatedLine).aliases);

                ui->widgetAddEditHost->show();

                selectionModel->clearSelection();
            }
            else if (action->data().toString() == "delete") {
                while (! selectionModel->selectedRows().isEmpty()) {
                    QModelIndex index = selectionModel->selectedRows().first();

                    int lineNumber = mSortFilterModel->index(index.row(), 0).data(9).toInt();

                    mHostFileContent.replace(lineNumber, "");

                    selectionModel->select(index, QItemSelectionModel::Deselect);
                }
                selectionModel->clearSelection();

                loadTableData();
            }
        }
    }
}
