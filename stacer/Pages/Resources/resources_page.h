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
    void updateMemoryChart();
    void updateNetworkChart();

private:
    Ui::ResourcesPage *ui;
    void init();

private:
    HistoryChart *cpuChart;
    HistoryChart *memoryChart;
    HistoryChart *networkChart;

    QTimer *timer;
    InfoManager *im;
};

#endif // RESOURCESPAGE_H
