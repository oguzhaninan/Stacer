#ifndef RESOURCESPAGE_H
#define RESOURCESPAGE_H

#include <QWidget>
#include <QTimer>

#include "history_chart.h"
#include "Managers/info_manager.h"
#include <Info/system_info.h>
#include <QChart>
#include <QSpacerItem>

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
    void updateGPUMemoryChart();
    void updateGPUUsageChart();
    void initDiskPieChart();
    void diskPieSeriesCustomize();

private:
    void init();

private:
    Ui::ResourcesPage *ui;

    InfoManager *im;
    SystemInfo si;

    HistoryChart *mChartCpu;
    HistoryChart *mChartCpuLoadAvg;
    HistoryChart *mChartDiskReadWrite;
    HistoryChart *mChartMemory;
    HistoryChart *mChartNetwork;
    HistoryChart *mChartGPUMemory;
    HistoryChart *mChartGPUUsage;

    QChartView *mChartViewDiskPie;
    QChart *mChartDiskPie;

    QWidget *gridWidgetDiskPie;
    QGridLayout *gridLayoutDiskPie;
    QPieSeries *mDiskPieSeries;

    QList<int> chartColors;

    QTimer *mTimer;
};

#endif // RESOURCESPAGE_H
