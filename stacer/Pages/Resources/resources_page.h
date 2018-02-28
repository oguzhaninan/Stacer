#ifndef RESOURCESPAGE_H
#define RESOURCESPAGE_H

#include <QWidget>
#include <QTimer>

#include "history_chart.h"
#include "Managers/info_manager.h"

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
    void updateMemoryChart();
    void updateNetworkChart();

private:
    Ui::ResourcesPage *ui;
    void init();

private:
    InfoManager *im;

    HistoryChart *cpuChart;
    HistoryChart *memoryChart;
    HistoryChart *networkChart;
    HistoryChart *cpuLoadAvgChart;

    QTimer *timer;
};

#endif // RESOURCESPAGE_H
