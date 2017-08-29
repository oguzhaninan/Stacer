#include "history_chart.h"
#include "ui_history_chart.h"

HistoryChart::~HistoryChart()
{
    delete ui;
}

HistoryChart::HistoryChart(QString title, int seriesCount, QWidget *parent) :
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
    ui->historyTitle->setText(title);

    // create lists
    for (int i = 0; i < seriesCount; i++)
        seriesList.append(new QLineSeries);

    // add series to chart
    for (int i = 0; i < seriesList.count(); ++i)
        chart->addSeries(seriesList.at(i));

    chartView->setRenderHint(QPainter::Antialiasing);

    QList<int> colors = {
        0x2ecc71, 0xe74c3c, 0x3498db, 0xf1c40f, 0xe67e22,
        0x1abc9c, 0x9b59b6, 0x34495e, 0xd35400, 0xc0392b,
        0x8e44ad, 0xFF8F00, 0xEF6C00, 0x4E342E, 0x424242,
        0x5499C7, 0x58D68D, 0xCD6155, 0xF5B041, 0x566573
    };
    // set colors
    for (int i = 0; i < seriesList.count(); ++i)
        seriesList.at(i)->setColor(QColor(colors.at(i)));

    // Chart Settings
    chart->createDefaultAxes();

    chart->axisX()->setRange(0, 60);
    chart->axisX()->setReverse(true);

    chart->setContentsMargins(-11, -11, -11, -11);
    chart->setMargins(QMargins(20, 0, 10, 10));
    ui->verticalLayout->addWidget(chartView);

    // theme changed
    connect(AppManager::ins(), &AppManager::changedTheme, this, [this]() {
        chart->axisX()->setLabelsColor(apm->getStyleValues()->value("@chartLabelColor").toString());
        chart->axisX()->setGridLineColor(apm->getStyleValues()->value("@chartGridColor").toString());

        chart->axisY()->setLabelsColor(apm->getStyleValues()->value("@chartLabelColor").toString());
        chart->axisY()->setGridLineColor(apm->getStyleValues()->value("@chartGridColor").toString());

        chart->setBackgroundBrush(QColor(apm->getStyleValues()->value("@historyChartBackgroundColor").toString()));
        chart->legend()->setLabelColor(apm->getStyleValues()->value("@chartLabelColor").toString());
    });
}

void HistoryChart::setYMax(int value)
{
    yMax = value;
}

QVector<QLineSeries *> HistoryChart::getSeriesList() const
{
    return seriesList;
}

void HistoryChart::setSeriesList(QVector<QLineSeries *> &value)
{
    seriesList = value;

    for (int i = 0; i < seriesList.count(); ++i)
        chart->series().replace(0, seriesList.at(i));

    if(yMax) chart->axisY()->setRange(0, yMax);

    chartView->repaint();
}

void HistoryChart::on_historyTitle_clicked(bool checked)
{
    QLayout *charts = topLevelWidget()->findChild<QWidget*>("charts")->layout();

    for (int i = 0; i < charts->count(); ++i)
        charts->itemAt(i)->widget()->setVisible(! checked);

    show();
}
