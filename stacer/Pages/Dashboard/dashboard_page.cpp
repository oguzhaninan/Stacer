#include "dashboard_page.h"
#include "ui_dashboard_page.h"

#include "utilities.h"

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
    im(InfoManager::ins()),
    iconTray(QIcon(":/static/logo.png")),
    mSettingManager(SettingManager::ins())
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
    timerDisk->start(5 * 1000);

    timer->start(1 * 1000);

    // initialization
    updateCpuBar();
    updateMemoryBar();
    updateDiskBar();
    updateNetworkBar();

    ui->widgetUpdateBar->hide();

    // check update
    checkUpdate();

    QList<QWidget*> widgets = {
        cpuBar, memBar, diskBar, downloadBar, uploadBar
    };
    Utilities::addDropShadow(widgets, 60);
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
                ui->widgetUpdateBar->show();
            }
        }
    }
}

void DashboardPage::on_btnDownloadUpdate_clicked()
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
        << tr("CPU Core: %1").arg(sysInfo.getCpuCore())
        << tr("CPU Speed: %1").arg(sysInfo.getCpuSpeed());

    QStringListModel *systemInfoModel = new QStringListModel(infos);

    ui->listViewSystemInfo->setModel(systemInfoModel);
}

void DashboardPage::updateCpuBar()
{
    int cpuUsedPercent = im->getCpuPercents().at(0);

    // alert message
    int maxCpuPercent = mSettingManager->getCpuAlertPercent();
    if (maxCpuPercent > 0) {
        static bool isShow = true;
        if (cpuUsedPercent > maxCpuPercent && isShow) {
            AppManager::ins()->getTrayIcon()->showMessage(tr("High CPU Usage"),
                                                          tr("The amount of CPU used is over %1%.").arg(maxCpuPercent),
                                                          iconTray);
            isShow = false;
        } else if (cpuUsedPercent < maxCpuPercent) {
            isShow = true;
        }
    }

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

    // alert message
    int maxMemoryPercent = mSettingManager->getMemoryAlertPercent();
    if (maxMemoryPercent > 0) {
        static bool isShow = true;
        if (memUsedPercent > maxMemoryPercent && isShow) {
            AppManager::ins()->getTrayIcon()->showMessage(tr("High Memory Usage"),
                                                          tr("The amount of memory used is over %1%.").arg(maxMemoryPercent),
                                                          iconTray);
            isShow = false;
        } else if (memUsedPercent < maxMemoryPercent) {
            isShow = true;
        }
    }

    memBar->setValue(memUsedPercent, QString("%1 / %2")
                     .arg(f_memUsed)
                     .arg(f_memTotal));
}

void DashboardPage::updateDiskBar()
{
    im->updateDiskInfo();

    if(! im->getDisks().isEmpty()) {
        Disk *disk = nullptr;
        QString selectedDiskName = mSettingManager->getDiskName();
        for (Disk *d: im->getDisks()) {
            if (d->name.trimmed() == selectedDiskName.trimmed())
                disk = d;
        }

        if (disk == nullptr) {
            disk = im->getDisks().at(0);
        }

        QString sizeText = FormatUtil::formatBytes(disk->size);
        QString usedText = FormatUtil::formatBytes(disk->used);

        int diskPercent = 0;
        if (disk->size > 0) {
            diskPercent = ((double) disk->used / (double) disk->size) * 100.0;
        }

        // alert message
        int maxDiskPercent = mSettingManager->getDiskAlertPercent();
        if (maxDiskPercent > 0) {
            static bool isShow = true;
            if (diskPercent > maxDiskPercent && isShow) {
                AppManager::ins()->getTrayIcon()->showMessage(tr("High Disk Usage"),
                                                              tr("The amount of disk used is over %1%.").arg(diskPercent),
                                                              iconTray);
                isShow = false;
            } else if (diskPercent < maxDiskPercent) {
                isShow = true;
            }
        }

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
    QString upText   = FormatUtil::formatBytes(d_TXbytes);

    int downPercent = ((double) d_RXbytes / (double) max_RXbytes) * 100.0;
    int upPercent   = ((double) d_TXbytes / (double) max_TXbytes) * 100.0;

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

