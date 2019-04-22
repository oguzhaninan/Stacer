 #include "resources_page.h"
#include "ui_resources_page.h"
#include "utilities.h"

ResourcesPage::~ResourcesPage()
{
    delete ui;
}

ResourcesPage::ResourcesPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResourcesPage),
    im(InfoManager::ins()),
    mChartCpu(new HistoryChart(tr("History of CPU"), im->getCpuCoreCount(), nullptr, this)),
    mChartCpuLoadAvg(new HistoryChart(tr("History of CPU Load Averages"), 3, nullptr, this)),
    mChartDiskReadWrite(new HistoryChart(tr("History of Disk Read Write"), 2, new QCategoryAxis, this)),
    mChartMemory(new HistoryChart(tr("History of Memory"), 2, nullptr, this)),
    mChartNetwork(new HistoryChart(tr("History of Network"), 2, new QCategoryAxis, this)),
    mTimer(new QTimer(this))
{
    ui->setupUi(this);

    init();
}

void ResourcesPage::init()
{
    mChartCpu->setYMax(100);
    mChartMemory->setYMax(100);

    QList<QWidget*> widgets = { mChartCpu, mChartCpuLoadAvg, mChartDiskReadWrite, mChartMemory, mChartNetwork };

    for (QWidget *widget : widgets) {
        ui->chartsLayout->addWidget(widget);
    }

    Utilities::addDropShadow(widgets, 40);

    connect(mTimer, &QTimer::timeout, this, &ResourcesPage::updateCpuChart);
    connect(mTimer, &QTimer::timeout, this, &ResourcesPage::updateCpuLoadAvg);
    connect(mTimer, &QTimer::timeout, this, &ResourcesPage::updateDiskReadWrite);
    connect(mTimer, &QTimer::timeout, this, &ResourcesPage::updateMemoryChart);
    connect(mTimer, &QTimer::timeout, this, &ResourcesPage::updateNetworkChart);

    mTimer->start(1000);


    QPieSeries *series = new QPieSeries();
    series->append("Jane", 1);
    series->append("Joe", 2);
    series->append("Andy", 3);
    series->append("Barbara", 4);
    series->append("Axel", 5);

    QPieSlice *slice = series->slices().at(1);
    slice->setExploded();
    slice->setLabelVisible();
    slice->setPen(QPen(Qt::darkGreen, 2));
    slice->setBrush(Qt::green);

    QChart *chart = new QChart();
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->addSeries(series);
    chart->setTitle("Simple piechart example");
    chart->setMinimumHeight(300);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->chartsLayout->addWidget(chartView);

}

void ResourcesPage::updateDiskReadWrite()
{
    static int second = 0;

    QList<quint64> diskReadWrite = im->getDiskIO();

    QVector<QSplineSeries*> seriesList = mChartDiskReadWrite->getSeriesList();

    for (int j = 0; j < seriesList.count(); ++j) {
        for (int i = 0; i < (second < 61 ? second : 61); ++i) {
            seriesList.at(j)->replace(i, (i+1), seriesList.at(j)->at(i).y());
        }

        if(second > 61) seriesList.at(j)->removePoints(61, 1);
    }

    static quint64 l_readBytes  = diskReadWrite.at(0); // last
    static quint64 l_writeBytes = diskReadWrite.at(1); // last
    static quint64 maxY = (1L << 10) * 100; // 100 KIBI

    quint64 readBytes  = diskReadWrite.at(0);
    quint64 writeBytes = diskReadWrite.at(1);

    quint64 d_readByte = (readBytes - l_readBytes);
    quint64 d_writeByte = (writeBytes - l_writeBytes);

    seriesList.at(0)->insert(0, QPointF(0, d_readByte));
    seriesList.at(0)->setName(tr("Read: %1/s Total: %2")
                              .arg(FormatUtil::formatBytes(d_readByte))
                              .arg(FormatUtil::formatBytes(readBytes)));


    seriesList.at(1)->insert(0, QPointF(0, d_writeByte));
    seriesList.at(1)->setName(tr("Write: %1/s Total: %2")
                              .arg(FormatUtil::formatBytes(d_writeByte))
                              .arg(FormatUtil::formatBytes(writeBytes)));

    maxY = qMax(qMax(maxY, d_readByte), d_writeByte);

    l_readBytes  = readBytes;
    l_writeBytes = writeBytes;

    mChartDiskReadWrite->setYMax(maxY);

    mChartDiskReadWrite->setSeriesList(seriesList);

    mChartDiskReadWrite->setCategoryAxisYLabels();

    second++;
}

void ResourcesPage::updateCpuLoadAvg()
{
    static int second, maxAvg = im->getCpuCoreCount();

    static int minutes[] = {1, 5, 15};

    QList<double> cpuLoadAvgs = im->getCpuLoadAvgs();

    QVector<QSplineSeries*> seriesList = mChartCpuLoadAvg->getSeriesList();

    for (int j = 0; j < seriesList.count(); ++j) {
        double avg = cpuLoadAvgs.at(j);

        for (int i = 0; i < (second < 61 ? second : 61); ++i) {
            seriesList.at(j)->replace(i, (i+1), seriesList.at(j)->at(i).y());
        }

        seriesList.at(j)->insert(0, QPointF(0, avg));

        seriesList.at(j)->setName(tr("%1 Minute Avarage: %2")
                                  .arg(minutes[j])
                                  .arg(avg));

        if (second > 61) seriesList.at(j)->removePoints(61, 1);

        maxAvg = qMax((int)ceil(avg), maxAvg);
    }

    mChartCpuLoadAvg->setYMax(maxAvg);

    mChartCpuLoadAvg->setSeriesList(seriesList);

    second++;
}

void ResourcesPage::updateNetworkChart()
{
    static int second = 0;

    QVector<QSplineSeries *> seriesList = mChartNetwork->getSeriesList();

    // points swap
    for (int j = 0; j < seriesList.count(); j++) {
        for (int i = 0; i < (second < 61 ? second : 61); i++)
            seriesList.at(j)->replace(i, (i+1), seriesList.at(j)->at(i).y());

        if(second > 61) seriesList.at(j)->removePoints(61, 1);
    }

    quint64 RXbytes = im->getRXbytes();
    quint64 TXbytes = im->getTXbytes();

    static quint64 l_RXbytes = RXbytes;
    static quint64 l_TXbytes = TXbytes;
    static quint64 max_RXbytes = 1L << 20; // 1 MEBI
    static quint64 max_TXbytes = 1L << 20; // 1 MEBI

    quint64 d_RXbytes = (RXbytes - l_RXbytes);
    quint64 d_TXbytes = (TXbytes - l_TXbytes);

    QString downText = FormatUtil::formatBytes(d_RXbytes);
    QString upText   = FormatUtil::formatBytes(d_TXbytes);

    // Download
    seriesList.at(0)->insert(0, QPointF(0, d_RXbytes));
    seriesList.at(0)->setName(tr("Download: %1/s Total: %2")
                              .arg(downText)
                              .arg(FormatUtil::formatBytes(RXbytes)));

    seriesList.at(1)->insert(0, QPointF(0, d_TXbytes));
    seriesList.at(1)->setName(tr("Upload: %1/s  Total: %2")
                              .arg(upText)
                              .arg(FormatUtil::formatBytes(TXbytes)));

    max_RXbytes = qMax(max_RXbytes, d_RXbytes);
    max_TXbytes = qMax(max_TXbytes, d_TXbytes);

    int max = qMax(max_RXbytes, max_TXbytes);

    l_RXbytes = RXbytes;
    l_TXbytes = TXbytes;

    mChartNetwork->setYMax(max);

    mChartNetwork->setSeriesList(seriesList);

    mChartNetwork->setCategoryAxisYLabels();

    second++;
}

void ResourcesPage::updateMemoryChart()
{
    static int second = 0;

    QVector<QSplineSeries *> seriesList = mChartMemory->getSeriesList();

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
    seriesList.at(0)->setName(tr("Swap: %1 (%2%) %3")
                              .arg(FormatUtil::formatBytes(im->getSwapUsed()))
                              .arg(QString().sprintf("%.1f",percent))
                              .arg(FormatUtil::formatBytes(im->getSwapTotal())));

    // Memory
    double percent2 = ((double) im->getMemUsed() / (double) im->getMemTotal()) * 100.0;

    seriesList.at(1)->insert(0, QPointF(0, percent2));
    seriesList.at(1)->setName(tr("Memory: %1 (%2%) %3")
                              .arg(FormatUtil::formatBytes(im->getMemUsed()))
                              .arg(QString().sprintf("%.1f",percent2))
                              .arg(FormatUtil::formatBytes(im->getMemTotal())));

    mChartMemory->setSeriesList(seriesList);

    second++;
}

void ResourcesPage::updateCpuChart()
{
    static int second = 0;

    QList<int> cpuPercents = im->getCpuPercents();

    QVector<QSplineSeries *> seriesList = mChartCpu->getSeriesList();

    for (int j = 0; j < seriesList.count(); j++){
        int p = cpuPercents.at(j+1);

        for (int i = 0; i < (second < 61 ? second : 61); i++)
            seriesList.at(j)->replace(i, (i+1), seriesList.at(j)->at(i).y());

        seriesList.at(j)->insert(0, QPointF(0, p));

        seriesList.at(j)->setName(QString("CPU%1: %2%").arg(j+1).arg(p));

        if(second > 61) seriesList.at(j)->removePoints(61, 1);
    }

    mChartCpu->setSeriesList(seriesList);

    second++;
}
