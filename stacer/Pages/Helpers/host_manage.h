#ifndef HOST_MANAGE_H
#define HOST_MANAGE_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QMenu>
#include "Utils/file_util.h"
#include "Utils/command_util.h"

#include "utilities.h"

namespace Ui {
class HostManage;
}

class HostItem
{
public:
    QString ip;
    QString fullQualified;
    QString aliases;
};

class HostManage : public QWidget
{
    Q_OBJECT

public:
    explicit HostManage(QWidget *parent = 0);
    ~HostManage();

private slots:
    void init();

    void on_btnNewHost_clicked();
    void on_btnSave_clicked();
    void loadHostItems();
    void loadTableData();
    void on_btnCancel_clicked();
    void loadTableRowMenu();
    void on_btnSaveChanges_clicked();
    QList<QStandardItem *> createRow(const QPair<int, HostItem> &item);

    void on_tableViewHosts_customContextMenuRequested(const QPoint &pos);

private:
    Ui::HostManage *ui;

    bool isAddHost;
    QList<QString> mHeaderList;

    QStandardItemModel *mItemModel;
    QSortFilterProxyModel *mSortFilterModel;
    QMenu mTableRowMenu;

    QStringList mHostFileContent;
    QMap<int, HostItem> mHostItemList;

    int updatedLine;
};

#endif // HOST_MANAGE_H
