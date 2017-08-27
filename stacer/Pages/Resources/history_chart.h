#ifndef HISTORYCHART_H
#define HISTORYCHART_H

#include <QWidget>
#include <QDebug>
#include <QtCharts>
#include <QTimer>

#include "Managers/app_manager.h"

namespace Ui {
    class HistoryChart;
}

class HistoryChart : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryChart(QString title, int seriesCount, QWidget *parent = 0);
    ~HistoryChart();

    QVector<QLineSeries *> getSeriesList() const;

public slots:
    void setYMax(int value);
    void setSeriesList(QVector<QLineSeries *> &value);

private slots:
    void init();
    void on_historyTitle_clicked(bool checked);

private:
    Ui::HistoryChart *ui;

private:
    QString title;
    int yMax;
    int seriesCount;
    QChartView *chartView;
    QChart *chart;
    QVector<QLineSeries *> seriesList;
    AppManager *apm;
};

#endif // HISTORYCHART_H
