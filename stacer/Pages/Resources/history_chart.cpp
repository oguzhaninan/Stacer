#include "history_chart.h"
#include "ui_history_chart.h"

HistoryChart::~HistoryChart()
{
    delete ui;
}

HistoryChart::HistoryChart(const QString &title, const int &seriesCount, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryChart),
    title(title),
    yMax(0),
    seriesCount(seriesCount),
    chartView(new QChartView(this)),
    chart(chartView->chart()),
    apm(AppManager::ins())
{
    ui->setupUi(this);

    init();
}

void HistoryChart::init()
{
    ui->lblHistoryTitle->setText(title);

    // create lists
    for (int i = 0; i < seriesCount; i++) {
        seriesList.append(new QSplineSeries);
    }

    // add series to chart
    for (int i = 0; i < seriesList.count(); ++i) {
        chart->addSeries(seriesList.at(i));
    }

    chartView->setRenderHint(QPainter::Antialiasing);

    QList<int> colors = {
        0x2ecc71, 0xe74c3c, 0x3498db, 0xf1c40f, 0xe67e22,
        0x1abc9c, 0x9b59b6, 0x34495e, 0xd35400, 0xc0392b,
        0x8e44ad, 0xFF8F00, 0xEF6C00, 0x4E342E, 0x424242,
        0x5499C7, 0x58D68D, 0xCD6155, 0xF5B041, 0x566573
    };
    // set colors
    for (int i = 0; i < seriesList.count(); ++i) {
        seriesList.at(i)->setColor(QColor(colors.at(i)));
    }

    // Chart Settings
    chart->createDefaultAxes();

    chart->axisX()->setRange(0, 60);
    chart->axisX()->setReverse(true);

    chart->setContentsMargins(-11, -11, -11, -11);
    chart->setMargins(QMargins(20, 0, 10, 10));
    ui->layoutHistoryChart->addWidget(chartView, 1, 0, 1, 3);

    // theme changed
    connect(SignalMapper::ins(), &SignalMapper::changedAppTheme, [=] {
        QString chartLabelColor = apm->getStyleValues()->value("@chartLabelColor").toString();
        QString chartGridColor = apm->getStyleValues()->value("@chartGridColor").toString();
        QString historyChartBackground = apm->getStyleValues()->value("@historyChartBackgroundColor").toString();

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
    yMax = value;
}

QVector<QSplineSeries *> HistoryChart::getSeriesList() const
{
    return seriesList;
}

void HistoryChart::setSeriesList(const QVector<QSplineSeries *> &value)
{
    seriesList = value;

    for (int i = 0; i < seriesList.count(); ++i)
        chart->series().replace(0, seriesList.at(i));

    if(yMax) chart->axisY()->setRange(0, yMax);

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
