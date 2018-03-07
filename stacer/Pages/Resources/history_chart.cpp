#include "history_chart.h"
#include "ui_history_chart.h"

HistoryChart::~HistoryChart()
{
    delete ui;
}

HistoryChart::HistoryChart(const QString &title, const int &seriesCount, QCategoryAxis* categoriAxisY, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryChart),
    title(title),
    seriesCount(seriesCount),
    chartView(new QChartView(this)),
    chart(chartView->chart())
{
    ui->setupUi(this);

    init();

    if (categoriAxisY) {
        axisY = categoriAxisY;
        axisY->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
        for (int i = 0; i < seriesCount; ++i) {
            chart->setAxisY(axisY, chart->series().at(i));
        }
    }
}

void HistoryChart::init()
{
    ui->lblHistoryTitle->setText(title);

    // add series to chart
    for (int i = 0; i < seriesCount; i++) {
        mSeriesList.append(new QSplineSeries);
        chart->addSeries(mSeriesList.at(i));
    }

    chartView->setRenderHint(QPainter::Antialiasing);

    QList<int> colors = {
        0x2ecc71, 0xe74c3c, 0x3498db, 0xf1c40f, 0xe67e22,
        0x1abc9c, 0x9b59b6, 0x34495e, 0xd35400, 0xc0392b,
        0x8e44ad, 0xFF8F00, 0xEF6C00, 0x4E342E, 0x424242,
        0x5499C7, 0x58D68D, 0xCD6155, 0xF5B041, 0x566573
    };
    // set colors
    for (int i = 0; i < chart->series().count(); ++i) {
        dynamic_cast<QSplineSeries*>(chart->series().at(i))->setColor(QColor(colors.at(i)));
    }

    // Chart Settings
    chart->createDefaultAxes();

    chart->axisX()->setRange(0, 60);
    chart->axisX()->setReverse(true);

    chart->setContentsMargins(-11, -11, -11, -11);
    chart->setMargins(QMargins(20, 0, 10, 10));
    ui->layoutHistoryChart->addWidget(chartView, 1, 0, 1, 3);

    // theme changed
    connect(SignalMapper::ins(), &SignalMapper::sigChangedAppTheme, [=] {
        QString chartLabelColor = AppManager::ins()->getStyleValues()->value("@chartLabelColor").toString();
        QString chartGridColor = AppManager::ins()->getStyleValues()->value("@chartGridColor").toString();
        QString historyChartBackground = AppManager::ins()->getStyleValues()->value("@historyChartBackgroundColor").toString();

        chart->axisX()->setLabelsColor(chartLabelColor);
        chart->axisX()->setGridLineColor(chartGridColor);

        chart->axisY()->setLabelsColor(chartLabelColor);
        chart->axisY()->setGridLineColor(chartGridColor);

        chart->setBackgroundBrush(QColor(historyChartBackground));
        chart->legend()->setLabelColor(chartLabelColor);
    });
}

void HistoryChart::setYMax(const int &value)
{
    chart->axisY()->setRange(0, value);
}

QCategoryAxis *HistoryChart::getAxisY()
{
    return axisY;
}

void HistoryChart::setCategoryAxisYLabels()
{
    if (axisY) {
        for (const QString &label : axisY->categoriesLabels()){
            axisY->remove(label);
        }

        for (int i = 1; i < 5; ++i) {
            axisY->append(FormatUtil::formatBytes((axisY->max()/4)*i), (axisY->max()/4)*i);
        }
    }
}

QVector<QSplineSeries*> HistoryChart::getSeriesList() const
{
    return mSeriesList;
}

void HistoryChart::setSeriesList(const QVector<QSplineSeries *> &seriesList)
{
    for (int i = 0; i < seriesList.count(); ++i) {
        chart->series().replace(0, seriesList.at(i));
    }

    chartView->repaint();
}

void HistoryChart::on_checkHistoryTitle_clicked(bool checked)
{
    QLayout *charts = topLevelWidget()->findChild<QWidget*>("charts")->layout();

    for (int i = 0; i < charts->count(); ++i) {
        charts->itemAt(i)->widget()->setVisible(! checked);
    }

    show();
}
