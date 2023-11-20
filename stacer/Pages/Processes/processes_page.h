#ifndef PROCESSESPAGE_H
#define PROCESSESPAGE_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QDebug>
#include <QScrollBar>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QPushButton>

#include "Managers/info_manager.h"

namespace Ui {
    class ProcessesPage;
}

class ProcessesPage : public QWidget
{
    Q_OBJECT

public:
    explicit ProcessesPage(QWidget *parent = 0);
    ~ProcessesPage();
    static void onLimitProcessConfirm(int limitValue, QString currentOptionName);

private slots:
    void init();
    void loadProcesses();
    void loadHeaderMenu();
    QList<QStandardItem *> createRow(const Process &proc);
    void on_txtProcessSearch_textChanged(const QString &val);
    void on_sliderRefresh_valueChanged(const int &i);
    void on_btnLimitProcess_clicked(); // ui file: line 205
    void on_btnRemoveLimit_clicked(); // ui file: line 221
    void on_btnEndProcess_clicked(); // ui file: line 237
    void on_tableProcess_customContextMenuRequested(const QPoint &pos);

private:
    Ui::ProcessesPage *ui;
    QStandardItemModel *mItemModel;
    QSortFilterProxyModel *mSortFilterModel;
    QModelIndex mSeletedRowModel;
    QStringList mHeaders;
    QMenu mHeaderMenu;
    QTimer *mTimer;
    InfoManager *im;
    int limitPass;
};

#endif // PROCESSESPAGE_H
