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
    cpuChart(new HistoryChart(tr("History of CPU"), im->getCpuCoreCount(), this)),
    cpuLoadAvgChart(new HistoryChart(tr("History of CPU Load Averages"), 3, this)),
    diskReadWriteChart(new HistoryChart(tr("History of Disk Read Write"), 2, this)),
    memoryChart(new HistoryChart(tr("History of Memory"), 2, this)),
    networkChart(new HistoryChart(tr("History of Network"), 2, this)),
    timer(new QTimer(this))
{
    ui->setupUi(this);

    init();
}

void ResourcesPage::init()
{
    cpuChart->setYMax(100);
    memoryChart->setYMax(100);
    cpuLoadAvgChart->setYMax(5);

    QList<QWidget*> widgets = { cpuChart, cpuLoadAvgChart, diskReadWriteChart, memoryChart, networkChart };

    for (QWidget *widget : widgets) {
        ui->chartsLayout->addWidget(widget);
    }

    Utilities::addDropShadow(widgets, 60);

    connect(timer, &QTimer::timeout, this, &ResourcesPage::updateCpuChart);
    connect(timer, &QTimer::timeout, this, &ResourcesPage::updateCpuLoadAvg);
    connect(timer, &QTimer::timeout, this, &ResourcesPage::updateDiskReadWrite);
    connect(timer, &QTimer::timeout, this, &ResourcesPage::updateMemoryChart);
    connect(timer, &QTimer::timeout, this, &ResourcesPage::updateNetworkChart);

    timer->start(1000);
}

void ResourcesPage::updateDiskReadWrite()
{
    static int second = 0;

    QList<quint64> diskReadWrite = im->getDiskIO();

    QVector<QSplineSeries*> seriesList = diskReadWriteChart->getSeriesList();

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

    seriesList.at(0)->insert(0, QPointF(0, d_readByte >> 10));
    seriesList.at(0)->setName(tr("Read %1/s Total: %2")
                              .arg(FormatUtil::formatBytes(d_readByte))
                              .arg(FormatUtil::formatBytes(readBytes)));


    seriesList.at(1)->insert(0, QPointF(0, d_writeByte >> 10));
    seriesList.at(1)->setName(tr("Write %1/s Total: %2")
                              .arg(FormatUtil::formatBytes(d_writeByte))
                              .arg(FormatUtil::formatBytes(writeBytes)));

    maxY = qMax(qMax(maxY, d_readByte), d_writeByte);

    diskReadWriteChart->setYMax(maxY >> 10);

    l_readBytes  = readBytes;
    l_writeBytes = writeBytes;

    second++;

    diskReadWriteChart->setSeriesList(seriesList);
}

void ResourcesPage::updateCpuLoadAvg()
{
    static int second, maxAvg = im->getCpuCoreCount();

    static int minutes[] = {1, 5, 15};

    QList<double> cpuLoadAvgs = im->getCpuLoadAvgs();

    QVector<QSplineSeries*> seriesList = cpuLoadAvgChart->getSeriesList();

    for (int j = 0; j < seriesList.count(); ++j) {
        double avg = cpuLoadAvgs.at(j);

        for (int i = 0; i < (second < 61 ? second : 61); ++i) {
            seriesList.at(j)->replace(i, (i+1), seriesList.at(j)->at(i).y());
        }

        seriesList.at(j)->insert(0, QPointF(0, avg));

        seriesList.at(j)->setName(tr("%1 Minute Avarage").arg(minutes[j]));

        if (second > 61) seriesList.at(j)->removePoints(61, 1);

        maxAvg = qMax((int)ceil(avg), maxAvg);
    }

    cpuLoadAvgChart->setYMax(maxAvg);

    second++;

    cpuLoadAvgChart->setSeriesList(seriesList);
}

void ResourcesPage::updateNetworkChart()
{
    static int second = 0;

    QVector<QSplineSeries *> seriesList = networkChart->getSeriesList();

    // points swap
    for (int j = 0; j < seriesList.count(); j++) {
        for (int i = 0; i < (second < 61 ? second : 61); i++)
            seriesList.at(j)->replace(i, (i+1), seriesList.at(j)->at(i).y());

        if(second > 61) seriesList.at(j)->removePoints(61, 1);
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
    QString upText   = FormatUtil::formatBytes(d_TXbytes);

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

    QVector<QSplineSeries *> seriesList = memoryChart->getSeriesList();

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

    QVector<QSplineSeries *> seriesList = cpuChart->getSeriesList();

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
