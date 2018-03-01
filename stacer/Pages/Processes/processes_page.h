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

private slots:
    void init();
    void loadProcesses();
    QList<QStandardItem *> createRow(const Process &proc);
    void on_txtProcessSearch_textChanged(const QString &val);
    void on_sliderRefresh_valueChanged(const int &i);
    void on_btnEndProcess_clicked();
    void on_tableProcess_customContextMenuRequested(const QPoint &pos);
    void loadHeaderMenu();

private:
    Ui::ProcessesPage *ui;

private:
    QStandardItemModel *model;
    QSortFilterProxyModel *sortModel;
    QModelIndex seletedRowModel;
    QStringList headers;
    InfoManager *im;
    QTimer *timer;
    QMenu headerMenu;
};

#endif // PROCESSESPAGE_H
