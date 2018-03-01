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
    explicit HistoryChart(const QString &title, const int &seriesCount, QWidget *parent = 0);
    ~HistoryChart();

    QVector<QSplineSeries *> getSeriesList() const;

public slots:
    void setYMax(const int &value);
    void setSeriesList(const QVector<QSplineSeries *> &value);

private slots:
    void init();
    void on_checkHistoryTitle_clicked(bool checked);

private:
    Ui::HistoryChart *ui;

private:
    QString title;
    int yMax;
    int seriesCount;
    QChartView *chartView;
    QChart *chart;
    QVector<QSplineSeries *> seriesList;
    AppManager *apm;
};

#endif // HISTORYCHART_H
