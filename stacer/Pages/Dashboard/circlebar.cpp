#include "circlebar.h"
#include "ui_circlebar.h"

CircleBar::~CircleBar()
{
    delete ui;
    delete chart;
    delete chartView;
    delete series;
}

CircleBar::CircleBar(QString title, QStringList colors, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CircleBar),
    colors(colors),
    chart(new QChart),
    chartView(new QChartView(chart)),
    series(new QPieSeries)
{
    ui->setupUi(this);

    ui->chartTitle->setText(title);

    init();
}

void CircleBar::init()
{
    // series settings
    series->setHoleSize(0.67);
    series->setPieSize(165);
    series->setPieStartAngle(-115);
    series->setPieEndAngle(115);
    series->setLabelsVisible(false);
    series->append("1", 0);
    series->append("2", 0);
    series->slices().first()->setBorderColor("transparent");
    series->slices().last()->setBorderColor("transparent");
    QConicalGradient gradient;
    gradient.setAngle(115);
    for (int i = 0; i < colors.count(); ++i)
        gradient.setColorAt(i, QColor(colors.at(i)));
    series->slices().first()->setBrush(gradient);

    // chart settings
    chart->setBackgroundBrush(QBrush("transparent"));
    chart->setContentsMargins(-15, -18, -15, -60);
    chart->addSeries(series);
    chart->legend()->hide();

    // chartview settings
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->chartLayout->insertWidget(1, chartView);

    connect(AppManager::ins(), &AppManager::changedTheme, this, [this](){
        chartView->setBackgroundBrush(QColor(AppManager::ins()->getStyleValues()->value("@circleChartBackgroundColor").toString()));

        series->slices().last()->setColor(AppManager::ins()->getStyleValues()->value("@pageContent").toString()); // trail color
    });
}

void CircleBar::setValue(int value, QString valueText)
{
    series->slices().first()->setValue(value);
    series->slices().last()->setValue(100 - value);

    ui->chartValue->setText(valueText);
}

