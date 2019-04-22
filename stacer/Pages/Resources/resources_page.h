#ifndef RESOURCESPAGE_H
#define RESOURCESPAGE_H

#include <QWidget>
#include <QTimer>

#include "history_chart.h"
#include "Managers/info_manager.h"
#include <QChart>

namespace Ui {
    class ResourcesPage;
}

class ResourcesPage : public QWidget
{
    Q_OBJECT

public:
    explicit ResourcesPage(QWidget *parent = 0);
    ~ResourcesPage();

private slots:
    void updateCpuChart();
    void updateCpuLoadAvg();
    void updateDiskReadWrite();
    void updateMemoryChart();
    void updateNetworkChart();

private:
    void init();

private:
    Ui::ResourcesPage *ui;

    InfoManager *im;

    HistoryChart *mChartCpu;
    HistoryChart *mChartCpuLoadAvg;
    HistoryChart *mChartDiskReadWrite;
    HistoryChart *mChartMemory;
    HistoryChart *mChartNetwork;

    QTimer *mTimer;
};

#endif // RESOURCESPAGE_H
