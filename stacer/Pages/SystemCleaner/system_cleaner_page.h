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
    explicit SystemCleanerPage(QWidget *parent = 0);
    ~SystemCleanerPage();

private slots:
    void init();
    void addTreeRoot(CleanCategories cat, QString title, QFileInfoList infos, bool noChild = false);
    void addTreeChild(CleanCategories cat, QString text, quint64 size);
    void addTreeChild(QString data, QString text, quint64 size, QTreeWidgetItem *parent);

    void on_scanResultTreeW_itemClicked(QTreeWidgetItem *item, int column);
    void on_cleanBtn_clicked();
    void on_scanBtn_clicked();
    void on_backButtton_clicked();

    void systemScan();
    void systemClean();
    bool cleanValid();

private:
    Ui::SystemCleanerPage *ui;

private:
    InfoManager *im;
    ToolManager *tmr;

    QIcon defaultIcon;
    QMovie *loadingMovie;    
    QMovie *loadingMovie_2;
};

#endif // SYSTEMCLEANERPAGE_H
