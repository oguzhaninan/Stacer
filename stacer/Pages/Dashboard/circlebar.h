#ifndef CIRCLEBAR_H
#define CIRCLEBAR_H

#include <QWidget>
#include <QtCharts>
#include "Managers/app_manager.h"

namespace Ui {
    class CircleBar;
}

class CircleBar : public QWidget
{
    Q_OBJECT

public:
    explicit CircleBar(QString title, QStringList colors, QWidget *parent = 0);
    ~CircleBar();

public slots:
    void setValue(int value, QString valueText);

private slots:
    void init();

private:
    Ui::CircleBar *ui;

private:
    QStringList colors;

    QChart *chart;
    QChartView *chartView;
    QPieSeries *series;
};

#endif // CIRCLEBAR_H
