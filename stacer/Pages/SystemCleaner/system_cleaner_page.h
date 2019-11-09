#ifndef SYSTEMCLEANERPAGE_H
#define SYSTEMCLEANERPAGE_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QMap>
#include <QMovie>
#include <QDebug>
#include <QDir>
#include <QtConcurrent>
#include <QThread>
#include "Managers/app_manager.h"

#include <Managers/info_manager.h>
#include <Managers/tool_manager.h>

namespace Ui {
    class SystemCleanerPage;
}

class SystemCleanerPage : public QWidget
{
    Q_OBJECT

public:
    enum CleanCategories {
        PACKAGE_CACHE,
        CRASH_REPORTS,
        APPLICATION_LOGS,
        APPLICATION_CACHES,
        TRASH
    };

public:
    explicit SystemCleanerPage(QWidget *parent = nullptr);
    ~SystemCleanerPage();

private slots:
    quint64 addTreeRoot(const CleanCategories &cat, const QString &title, const QFileInfoList &infos, bool noChild = false);
    void addTreeChild(const CleanCategories &cat, const QString &text, const quint64 &size);
    void addTreeChild(const QString &data, const QString &text, const quint64 &size, QTreeWidgetItem *parent);

    void on_treeWidgetScanResult_itemClicked(QTreeWidgetItem *item, const int &column);
    void on_btnClean_clicked();
    void on_btnScan_clicked();
    void on_btnBackToCategories_clicked();

    void systemScan();
    void systemClean();
    bool cleanValid();

    void on_checkSelectAllSystemScan_clicked(bool checked);
    void on_checkSelectAll_clicked(bool check);
    void on_cbSortBy_currentIndexChanged(int idx);

private:
    void init();

private:
    Ui::SystemCleanerPage *ui;

    InfoManager *im;
    ToolManager *tmr;

    QIcon mDefaultIcon;
    QMovie *mLoadingMovie;
    QMovie *mLoadingMovie_2;
};

#endif // SYSTEMCLEANERPAGE_H
