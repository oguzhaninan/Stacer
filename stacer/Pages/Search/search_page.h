#ifndef SEARCH_PAGE_H
#define SEARCH_PAGE_H

#include <QWidget>
#include <QFileDialog>
#include "Managers/info_manager.h"
#include "utilities.h"
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QAction>
#include <QDateTime>
#include "Utils/format_util.h"

namespace Ui {
class SearchPage;
}

class SearchPage : public QWidget
{
    Q_OBJECT

public:
    explicit SearchPage(QWidget *parent = 0);
    ~SearchPage();

private slots:
    void on_btnBrowseSearchDir_clicked();
    void init();

    void on_btnAdvancePaneToggle_clicked();
    void on_btnSearchAdvance_clicked();
    void initComboboxValues();
    void on_tableFoundResults_customContextMenuRequested(const QPoint &pos);
    void loadHeaderMenu();
    void loadDataToTable(const QList<QFileInfo> &results);
    QList<QStandardItem *> createRow(const QFileInfo &fileInfo);

private:
    Ui::SearchPage *ui;

    QString mSelectedDirectory;

    QStringList mTableHeaders;
    QStandardItemModel *mItemModel;
    QSortFilterProxyModel *mSortFilterModel;
    QModelIndex mSeletedRowModel;
    QMenu mHeaderMenu;
};

#endif // SEARCH_PAGE_H
