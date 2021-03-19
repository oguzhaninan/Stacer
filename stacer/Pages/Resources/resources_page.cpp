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
    si(SystemInfo()),
    mChartCpu(new HistoryChart(tr("History of CPU"), im->getCpuCoreCount(), nullptr, this)),
    mChartCpuLoadAvg(new HistoryChart(tr("History of CPU Load Averages"), 3, nullptr, this)),
    mChartGPUMemory(new HistoryChart(tr("History of GPU Memory"), si.getGPUCore(), nullptr, this)),
    mChartGPUUsage(new HistoryChart(tr("History of GPU Usage"), si.getGPUCore(), nullptr, this)),
    mChartDiskReadWrite(new HistoryChart(tr("History of Disk Read Write"), 2, new QCategoryAxis, this)),
    mChartMemory(new HistoryChart(tr("History of Memory"), 2, nullptr, this)),
    mChartNetwork(new HistoryChart(tr("History of Network"), 2, new QCategoryAxis, this)),
    mChartDiskPie(new QChart),
    mTimer(new QTimer(this))
{
    ui->setupUi(this);

    init();
}

void ResourcesPage::init()
{
    chartColors = {
        0x2ecc71, 0xe74c3c, 0x3498db, 0xf1c40f, 0xe67e22,
        0x1abc9c, 0x9b59b6, 0x34495e, 0xd35400, 0xc0392b,
        0x8e44ad, 0xFF8F00, 0xEF6C00, 0x4E342E, 0x424242,
        0x5499C7, 0x58D68D, 0xCD6155, 0xF5B041, 0x566573
    };

    mChartCpu->setYMax(100);
    mChartMemory->setYMax(100);

    QList<QWidget*> widgets = { mChartCpu, mChartCpuLoadAvg, mChartMemory, mChartDiskReadWrite, mChartNetwork };

    if(si.sucessGPUInfo){
        widgets.insert(2,mChartGPUMemory);
        widgets.insert(2,mChartGPUUsage);
    }

    for (QWidget *widget : widgets) {
        ui->chartsLayout->addWidget(widget);
    }

    Utilities::addDropShadow(widgets, 40);

    connect(mTimer, &QTimer::timeout, this, &ResourcesPage::updateCpuChart);
    connect(mTimer, &QTimer::timeout, this, &ResourcesPage::updateCpuLoadAvg);
    if(si.sucessGPUInfo){
        connect(mTimer, &QTimer::timeout, this, &ResourcesPage::updateGPUMemoryChart);
        mChartGPUMemory->setYMax(100);
        connect(mTimer, &QTimer::timeout, this, &ResourcesPage::updateGPUUsageChart);
        mChartGPUUsage->setYMax(100);
    }
    connect(mTimer, &QTimer::timeout, this, &ResourcesPage::updateDiskReadWrite);
    connect(mTimer, &QTimer::timeout, this, &ResourcesPage::updateMemoryChart);
    connect(mTimer, &QTimer::timeout, this, &ResourcesPage::updateNetworkChart);

    mTimer->start(1000);

    initDiskPieChart();
}

void ResourcesPage::diskPieSeriesCustomize()
{
    for (int i = 0; i < mDiskPieSeries->count(); ++i) {
        QPieSlice *slice = mDiskPieSeries->slices().at(i);
        slice->setBrush(QColor((i < chartColors.count() ? chartColors.at(i) : i - chartColors.count())));
        slice->setBorderColor(QColor(Qt::lightGray));
        connect(slice, &QPieSlice::hovered, this, [=](bool show) {
            slice->setExploded(show);
            mChartDiskPie->setTitle(QString("%1 (%2) (%3)")
                                    .arg(slice->label())
                                    .arg(FormatUtil::formatBytes(slice->value()))
                                    .arg(QString().sprintf("%1.2f%%", slice->percentage() * 100)));
        });
    }
}

void ResourcesPage::initDiskPieChart()
{
    mDiskPieSeries = new QPieSeries();

    for (const Disk *disk : InfoManager::ins()->getDisks()) {
        mDiskPieSeries->append(disk->name, disk->size);
    }

    diskPieSeriesCustomize();

    mChartDiskPie->legend()->hide();
    mChartDiskPie->setAnimationOptions(QChart::AllAnimations);
    mChartDiskPie->addSeries(mDiskPieSeries);
    mChartDiskPie->setMinimumHeight(500);

    QChartView *mChartViewDiskPie = new QChartView(mChartDiskPie);
    mChartViewDiskPie->setRenderHint(QPainter::Antialiasing);

    mChartDiskPie->setContentsMargins(-11, -11, -11, -11);
    mChartDiskPie->setMargins(QMargins(20, 10, 10, 10));

    gridWidgetDiskPie = new QWidget(this);
    gridLayoutDiskPie = new QGridLayout(gridWidgetDiskPie);
    gridWidgetDiskPie->setLayout(gridLayoutDiskPie);
    gridLayoutDiskPie->setContentsMargins(0,0,0,0);
    gridLayoutDiskPie->setHorizontalSpacing(10);
    gridLayoutDiskPie->setVerticalSpacing(5);

    QLabel *lblChartTitle = new QLabel(gridWidgetDiskPie);
    lblChartTitle->setObjectName("lblHistoryTitle");
    lblChartTitle->setText(tr("File System"));

    QCheckBox *checkHistoryTitle = new QCheckBox(gridWidgetDiskPie);
    checkHistoryTitle->setObjectName("checkHistoryTitle");
    checkHistoryTitle->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    connect(checkHistoryTitle, &QCheckBox::clicked, this, [=](bool checked) {
        QLayout *charts = topLevelWidget()->findChild<QWidget*>("charts")->layout();

        for (int i = 0; i < charts->count(); ++i) {
            charts->itemAt(i)->widget()->setVisible(! checked);
        }

        gridWidgetDiskPie->show();
    });

    QComboBox *cmbFileSystemType = new QComboBox(gridWidgetDiskPie);
    cmbFileSystemType->addItem(tr("File System Type"), -1);
    cmbFileSystemType->addItems(InfoManager::ins()->getFileSystemTypes());
    connect(cmbFileSystemType, &QComboBox::currentTextChanged, this, [=](const QString text) {
        mChartDiskPie->removeSeries(mDiskPieSeries);
        mDiskPieSeries = new QPieSeries();

        for (const Disk *disk : InfoManager::ins()->getDisks()) {
            if (cmbFileSystemType->currentIndex() != 0 && disk->fileSystemType == text) {
                mDiskPieSeries->append(disk->name, disk->size);
            } else if(cmbFileSystemType->currentIndex() == 0) {
                mDiskPieSeries->append(disk->name, disk->size);
            }
        }
        diskPieSeriesCustomize();
        emit SignalMapper::ins()->sigChangedAppTheme();

        mChartDiskPie->addSeries(mDiskPieSeries);
    });

    QComboBox *cmbDevice = new QComboBox(gridWidgetDiskPie);
    cmbDevice->addItem(tr("Device"));
    cmbDevice->addItems(InfoManager::ins()->getDevices());
    connect(cmbDevice, &QComboBox::currentTextChanged, this, [=](const QString text) {
        mChartDiskPie->removeSeries(mDiskPieSeries);
        mDiskPieSeries = new QPieSeries();

        for (const Disk *disk : InfoManager::ins()->getDisks()) {
            if (cmbDevice->currentIndex() != 0 && disk->device == text) {
                mDiskPieSeries->append(disk->name, disk->size);
            } else if(cmbDevice->currentIndex() == 0) {
                mDiskPieSeries->append(disk->name, disk->size);
            }
        }
        diskPieSeriesCustomize();
        emit SignalMapper::ins()->sigChangedAppTheme();

        mChartDiskPie->addSeries(mDiskPieSeries);
    });

    QSpacerItem *horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayoutDiskPie->addWidget(lblChartTitle, 0, 0);
    gridLayoutDiskPie->addWidget(checkHistoryTitle, 0, 1);
    gridLayoutDiskPie->addItem(horizontalSpacer, 0, 2);
    gridLayoutDiskPie->addWidget(cmbDevice, 0, 3);
    gridLayoutDiskPie->addWidget(cmbFileSystemType, 0, 4);

    gridLayoutDiskPie->addWidget(mChartViewDiskPie, 1, 0, 1, 5);

    ui->chartsLayout->addWidget(gridWidgetDiskPie);

    // theme changed
    connect(SignalMapper::ins(), &SignalMapper::sigChangedAppTheme, [=] {
        QString chartLabelColor = AppManager::ins()->getStyleValues()->value("@chartLabelColor").toString();
        QString chartGridColor = AppManager::ins()->getStyleValues()->value("@chartGridColor").toString();
        QString historyChartBackground = AppManager::ins()->getStyleValues()->value("@historyChartBackgroundColor").toString();

        for (int i = 0; i < mDiskPieSeries->count(); ++i) {
            mDiskPieSeries->slices().at(i)->setLabelBrush(QColor(chartGridColor));
        }

        mChartDiskPie->setBackgroundBrush(QColor(historyChartBackground));
        mChartDiskPie->legend()->setLabelColor(chartLabelColor);
        mChartDiskPie->setTitleBrush(QColor(chartGridColor));
    });
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

        seriesList.at(j)->setName(tr("%1 Minute Average: %2")
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
    if (im->getSwapTotal()) // arithmetic exception control
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

void ResourcesPage::updateGPUMemoryChart()
{
    static int second = 0;

    std::vector<double> GPUMemory = im->getGPUMemoryUsage();
    std::vector<double> GPUMemoryTotal = si.getGPUTotalMemory();

    QVector<QSplineSeries *> seriesList = mChartGPUMemory->getSeriesList();

    for (int j = 0; j < seriesList.count(); j++){
        double p = GPUMemory.at(j)/GPUMemoryTotal.at(j)*100;

        for (int i = 0; i < (second < 61 ? second : 61); i++)
            seriesList.at(j)->replace(i, (i+1), seriesList.at(j)->at(i).y());

        seriesList.at(j)->insert(0, QPointF(0, p));

        seriesList.at(j)->setName(si.getGPUName().at(j));

        if(second > 61) seriesList.at(j)->removePoints(61, 1);
    }

    mChartGPUMemory->setSeriesList(seriesList);

    second++;
}

void ResourcesPage::updateGPUUsageChart()
{
    static int second = 0;

    std::vector<double> GPUUsage = im->getGPUUsage();

    QVector<QSplineSeries *> seriesList = mChartGPUUsage->getSeriesList();

    for (int j = 0; j < seriesList.count(); j++){
        double p = GPUUsage.at(j);

        for (int i = 0; i < (second < 61 ? second : 61); i++)
            seriesList.at(j)->replace(i, (i+1), seriesList.at(j)->at(i).y());

        seriesList.at(j)->insert(0, QPointF(0, p));

        seriesList.at(j)->setName(si.getGPUName().at(j));

        if(second > 61) seriesList.at(j)->removePoints(61, 1);
    }

    mChartGPUUsage->setSeriesList(seriesList);

    second++;
}
