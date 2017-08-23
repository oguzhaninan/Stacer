#include "resources_page.h"
#include "ui_resources_page.h"

ResourcesPage::~ResourcesPage()
{
    delete ui;
    delete cpuChart;
    delete memoryChart;
    delete networkChart;
    delete timer;
}

ResourcesPage::ResourcesPage(QWidget *parent) :
    QWidget(parent),
    timer(new QTimer),
    im(InfoManager::ins()),
    ui(new Ui::ResourcesPage)
{
    ui->setupUi(this);

    init();
}

void ResourcesPage::init()
{
    cpuChart = new HistoryChart(tr("CPU History"), im->getCpuCoreCount());
    cpuChart->setYMax(100);

    memoryChart = new HistoryChart(tr("Memory History"), 2);
    memoryChart->setYMax(100);

    networkChart = new HistoryChart(tr("Network History"), 2);

    ui->chartsLayout->addWidget(cpuChart);
    ui->chartsLayout->addWidget(memoryChart);
    ui->chartsLayout->addWidget(networkChart);

    connect(timer, &QTimer::timeout, this, &ResourcesPage::updateCpuChart);
    connect(timer, &QTimer::timeout, this, &ResourcesPage::updateMemoryChart);
    connect(timer, &QTimer::timeout, this, &ResourcesPage::updateNetworkChart);

    timer->start(1 * 1000);
}

void ResourcesPage::updateNetworkChart()
{
    static int second = 0;

    QVector<QLineSeries *> seriesList = networkChart->getSeriesList();

    // points swap
    for (int j = 0; j < seriesList.count(); j++) {
        for (int i = 0; i < (second < 61 ? second : 61); i++)
            seriesList.at(j)->replace(i, (i+1), seriesList.at(j)->at(i).y());

        if(second > 61)
            seriesList.at(j)->removePoints(61, 1);
    }

    static quint64 l_RXbytes = im->getRXbytes();
    static quint64 l_TXbytes = im->getTXbytes();
    static quint64 max_RXbytes = 1L << 20; // 1 MEBI
    static quint64 max_TXbytes = 1L << 20; // 1 MEBI

    quint64 RXbytes = im->getRXbytes();
    quint64 TXbytes = im->getTXbytes();

    quint64 d_RXbytes = (RXbytes - l_RXbytes);
    quint64 d_TXbytes = (TXbytes - l_TXbytes);

    QString downText = FormatUtil::formatBytes(d_RXbytes);
    QString upText = FormatUtil::formatBytes(d_TXbytes);

    // Download
    seriesList.at(0)->insert(0, QPointF(0, d_RXbytes >> 10));
    seriesList.at(0)->setName(tr("Download %1/s Total: %2")
                              .arg(downText)
                              .arg(FormatUtil::formatBytes(RXbytes)));

    seriesList.at(1)->insert(0, QPointF(0, d_TXbytes >> 10));
    seriesList.at(1)->setName(tr("Upload %1/s  Total: %2")
                              .arg(upText)
                              .arg(FormatUtil::formatBytes(TXbytes)));

    max_RXbytes = qMax(max_RXbytes, d_RXbytes);
    max_TXbytes = qMax(max_TXbytes, d_TXbytes);

    int max = qMax(max_RXbytes, max_TXbytes) >> 10;
    networkChart->setYMax(max);

    l_RXbytes = RXbytes;
    l_TXbytes = TXbytes;

    second++;

    networkChart->setSeriesList(seriesList);
}

void ResourcesPage::updateMemoryChart()
{
    static int second = 0;

    QVector<QLineSeries *> seriesList = memoryChart->getSeriesList();

    im->updateMemoryInfo();

    // points swap
    for (int j = 0; j < seriesList.count(); j++) {
        for (int i = 0; i < (second < 61 ? second : 61); i++)
            seriesList.at(j)->replace(i, (i+1), seriesList.at(j)->at(i).y());

        if(second > 61)
            seriesList.at(j)->removePoints(61, 1);
    }

    // Swap
    double percent = 0;
    if (im->getSwapTotal()) // aritmetic exception control
        percent = ((double) im->getSwapUsed() / (double) im->getSwapTotal()) * 100.0;

    seriesList.at(0)->insert(0, QPointF(0, percent));
    seriesList.at(0)->setName(tr("Swap %1 (%2%) %3")
                              .arg(FormatUtil::formatBytes(im->getSwapUsed()))
                              .arg(QString().sprintf("%.1f",percent))
                              .arg(FormatUtil::formatBytes(im->getSwapTotal())));

    // Memory
    double percent2 = ((double) im->getMemUsed() / (double) im->getMemTotal()) * 100.0;

    seriesList.at(1)->insert(0, QPointF(0, percent2));
    seriesList.at(1)->setName(tr("Memory %1 (%2%) %3")
                              .arg(FormatUtil::formatBytes(im->getMemUsed()))
                              .arg(QString().sprintf("%.1f",percent2))
                              .arg(FormatUtil::formatBytes(im->getMemTotal())));

    second++;

    memoryChart->setSeriesList(seriesList);
}

void ResourcesPage::updateCpuChart()
{
    static int second = 0;

    QList<int> cpuPercents = im->getCpuPercents();

    QVector<QLineSeries *> seriesList = cpuChart->getSeriesList();

    for (int j = 0; j < seriesList.count(); j++){
        int p = cpuPercents.at(j+1);

        for (int i = 0; i < (second < 61 ? second : 61); i++)
            seriesList.at(j)->replace(i, (i+1), seriesList.at(j)->at(i).y());

        seriesList.at(j)->insert(0, QPointF(0, p));

        seriesList.at(j)->setName(QString("CPU%1 %2%").arg(j+1).arg(p));

        if(second > 61) seriesList.at(j)->removePoints(61, 1);
    }

    second++;

    cpuChart->setSeriesList(seriesList);
}
