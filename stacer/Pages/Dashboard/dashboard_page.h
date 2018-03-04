#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDesktopServices>
#include <QtConcurrent>

#include <Managers/info_manager.h>
#include "circlebar.h"
#include "linebar.h"

namespace Ui {
    class DashboardPage;
}

class DashboardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardPage(QWidget *parent = 0);
    ~DashboardPage();

private slots:
    void init();
    void checkUpdate();
    void systemInformationInit();

    void updateCpuBar();
    void updateMemoryBar();
    void updateDiskBar();
    void updateNetworkBar();

    void on_btnDownloadUpdate_clicked();

private:
    Ui::DashboardPage *ui;

private:
    CircleBar* cpuBar;
    CircleBar* memBar;
    CircleBar* diskBar;

    LineBar *downloadBar;
    LineBar *uploadBar;

    QTimer *timer;
    InfoManager *im;

    QIcon iconTray;
};

#endif // DASHBOARDPAGE_H
