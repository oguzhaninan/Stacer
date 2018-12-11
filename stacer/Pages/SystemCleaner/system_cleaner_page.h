#ifndef SYSTEMCLEANERPAGE_H
#define SYSTEMCLEANERPAGE_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QMap>
#include <QMovie>
#include <QDebug>
#include <QtConcurrent>
#include <QThread>
#include "Managers/app_manager.h"

#include <Managers/info_manager.h>
#include <Managers/tool_manager.h>

#include "Types/metacallback.hpp"
#include "Types/Applications/broken_app.h"
#include "Types/Applications/desktop.h"

namespace Ui {
    class SystemCleanerPage;
}

typedef void(*typeCallback)(QTreeWidgetItem*,bool);
typedef typename Types::MetaCallback<::typeCallback> scpCallback;

Q_DECLARE_METATYPE(Types::MetaCallback<::typeCallback>)

class SystemCleanerPage : public QWidget
{
    Q_OBJECT

public:
    enum CleanCategories {
        PACKAGE_CACHE,
        CRASH_REPORTS,
        APPLICATION_LOGS,
        APPLICATION_CACHES,
        TRASH,
        BROKEN_APPLICATIONS
    };

public:
    explicit SystemCleanerPage(QWidget *parent = 0);
    ~SystemCleanerPage();

signals:
    void treeInvalidated(QTreeWidget *tree);

private slots:
    void addTreeRoot(const CleanCategories &cat, const QString &title, const QFileInfoList &infos, bool noChild = false);
    void addCallbackRoot(const QString &title, typeCallback callback, QTreeWidgetItem **out = nullptr);
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

    void invalidateTree(QTreeWidget *tree);
    void when_destroyed();

    void on_checkBrokenApps_stateChanged(int state);
    void on_checkBrokenApps_clicked();

private:
    void init();

private:
    Ui::SystemCleanerPage *ui;

    InfoManager *im;
    ToolManager *tmr;

    QIcon mDefaultIcon;
    QMovie *mLoadingMovie;
    QMovie *mLoadingMovie_2;

    /*
     * this counter still didnt help
     * i must assume that the cause of the sigsegv on exit
     * is the fault of the threads that spawn on sys scan
     */
    static int    mDestructed;
};

#endif // SYSTEMCLEANERPAGE_H
