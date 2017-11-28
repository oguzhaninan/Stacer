#include "dashboard_page.h"
#include "ui_dashboard_page.h"

DashboardPage::~DashboardPage()
{
    delete ui;
}

DashboardPage::DashboardPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DashboardPage),
    cpuBar(new CircleBar(tr("CPU"), {"#A8E063", "#56AB2F"}, this)),
    memBar(new CircleBar(tr("MEMORY"), {"#FFB75E", "#ED8F03"}, this)),
    diskBar(new CircleBar(tr("DISK"), {"#DC2430", "#7B4397"}, this)),
    downloadBar(new LineBar(tr("DOWNLOAD"), this)),
    uploadBar(new LineBar(tr("UPLOAD"), this)),
    timer(new QTimer(this)),
    im(InfoManager::ins())
{
    ui->setupUi(this);

    init();

    systemInformationInit();
}

void DashboardPage::init()
{
    // Circle bars
    ui->circleBarsLayout->addWidget(cpuBar);
    ui->circleBarsLayout->addWidget(memBar);
    ui->circleBarsLayout->addWidget(diskBar);

    // line bars
    ui->lineBarsLayout->addWidget(downloadBar);
    ui->lineBarsLayout->addWidget(uploadBar);

    // connections
    connect(timer, &QTimer::timeout, this, &DashboardPage::updateCpuBar);
    connect(timer, &QTimer::timeout, this, &DashboardPage::updateMemoryBar);
    connect(timer, &QTimer::timeout, this, &DashboardPage::updateNetworkBar);

    QTimer *timerDisk = new QTimer;
    connect(timerDisk, &QTimer::timeout, this, &DashboardPage::updateDiskBar);
    timerDisk->start(10 * 1000);

    timer->start(1 * 1000);

    // initialization
    updateCpuBar();
    updateMemoryBar();
    updateDiskBar();
    updateNetworkBar();

    ui->updateBarWidget->hide();

    // check update
    checkUpdate();
}

void DashboardPage::checkUpdate()
{
    QString requestResult;

    if (CommandUtil::isExecutable("curl")) {
        try {
            requestResult = CommandUtil::exec("curl", { "https://api.github.com/repos/oguzhaninan/Stacer/releases/latest" });
        } catch (QString &ex) {
            qCritical() << ex;
        }

        if (! requestResult.isEmpty()) {
            QJsonDocument result = QJsonDocument::fromJson(requestResult.toUtf8());

            QRegExp ex("[0-9].[0-9].[0-9]");
            ex.indexIn(result.object().value("tag_name").toString());

            QString version;
            if (! ex.captureCount())
                version = ex.cap();

            if (qApp->applicationVersion() != version) {
                ui->updateBarWidget->show();
            }
        }
    }
}

void DashboardPage::on_downloadUpdateBtn_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/oguzhaninan/Stacer/releases/latest"));
}

void DashboardPage::systemInformationInit()
{
    // get system information
    SystemInfo sysInfo;

    QStringList infos;
    infos
        << tr("Hostname: %1").arg(sysInfo.getHostname())
        << tr("Platform: %1").arg(sysInfo.getPlatform())
        << tr("Distribution: %1").arg(sysInfo.getDistribution())
        << tr("Kernel Release: %1").arg(sysInfo.getKernel())
        << tr("CPU Model: %1").arg(sysInfo.getCpuModel())
        << tr("CPU Speed (Mhz) : %1").arg(sysInfo.getCpuSpeed())
        << tr("CPU Core: %1").arg(sysInfo.getCpuCore());

    QStringListModel *systemInfoModel = new QStringListModel(infos);

    ui->systemInfoList->setModel(systemInfoModel);
}

void DashboardPage::updateCpuBar()
{
    int cpuUsedPercent = im->getCpuPercents().at(0);

    cpuBar->setValue(cpuUsedPercent, QString("%1%").arg(cpuUsedPercent));
}

void DashboardPage::updateMemoryBar()
{
    im->updateMemoryInfo();

    int memUsedPercent = 0;
    if (im->getMemTotal())
        memUsedPercent = ((double)im->getMemUsed() / (double)im->getMemTotal()) * 100.0;

    QString f_memUsed  = FormatUtil::formatBytes(im->getMemUsed());
    QString f_memTotal = FormatUtil::formatBytes(im->getMemTotal());

    memBar->setValue(memUsedPercent, QString("%1 / %2")
                     .arg(f_memUsed)
                     .arg(f_memTotal));
}

void DashboardPage::updateDiskBar()
{
    im->updateDiskInfo();

    if(! im->getDisks().isEmpty())
    {
        quint64 size = im->getDisks().at(0).size;
        quint64 used = im->getDisks().at(0).used;

        QString sizeText = FormatUtil::formatBytes(size);
        QString usedText = FormatUtil::formatBytes(used);

        int diskPercent = 0;
        if (size)
            diskPercent = ( (double) used / (double) size ) * 100.0;

        diskBar->setValue(diskPercent, QString("%1 / %2")
                          .arg(usedText)
                          .arg(sizeText));
    }
}

void DashboardPage::updateNetworkBar()
{
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

    int downPercent = ((double) d_RXbytes / (double) max_RXbytes) * 100.0;
    int upPercent = ((double) d_TXbytes / (double) max_TXbytes) * 100.0;

    downloadBar->setValue(downPercent,
                          QString("%1/s").arg(downText),
                          tr("Total: %1").arg(FormatUtil::formatBytes(RXbytes)));

    uploadBar->setValue(upPercent,
                        QString("%1/s").arg(upText),
                        tr("Total: %1").arg(FormatUtil::formatBytes(TXbytes)));

    max_RXbytes = qMax(max_RXbytes, d_RXbytes);
    max_TXbytes = qMax(max_TXbytes, d_TXbytes);

    l_RXbytes = RXbytes;
    l_TXbytes = TXbytes;
}

