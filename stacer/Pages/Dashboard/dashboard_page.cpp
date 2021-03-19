#include "dashboard_page.h"
#include "ui_dashboard_page.h"

#include <algorithm>
#include <numeric>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <numeric>

DashboardPage::~DashboardPage()
{
    delete ui;
}

DashboardPage::DashboardPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DashboardPage),
    mCpuBar(new CircleBar(tr("CPU"), {"#A8E063", "#56AB2F"}, this)),
    mMemBar(new CircleBar(tr("MEMORY"), {"#FFB75E", "#ED8F03"}, this)),
    mDiskBar(new CircleBar(tr("DISK"), {"#DC2430", "#7B4397"}, this)),
    mGPUUtilisationBar(new CircleBar(tr("GPU"), {"#7FFF00","#0000ff"}, this)),
    mGPUMemoryBar(new CircleBar(tr("GPU MEMORY"), {"#DEB887","#0000ff"}, this)),
    mDownloadBar(new LineBar(tr("DOWNLOAD"), this)),
    mUploadBar(new LineBar(tr("UPLOAD"), this)),
    mTimer(new QTimer(this)),
    im(InfoManager::ins()),
    mSettingManager(SettingManager::ins())
{
    ui->setupUi(this);

    init();

    systemInformationInit();
}

void DashboardPage::init()
{
    sysInfo = SystemInfo();
    // Circle bars
    ui->circleBarsLayout->addWidget(mCpuBar);
    ui->circleBarsLayout->addWidget(mMemBar);
    if(sysInfo.sucessGPUInfo){
        ui->circleBarsLayout->addWidget(mGPUUtilisationBar);
        ui->circleBarsLayout->addWidget(mGPUMemoryBar);
    }

    ui->circleBarsLayout->addWidget(mDiskBar);

    // line bars
    ui->lineBarsLayout->addWidget(mDownloadBar);
    ui->lineBarsLayout->addWidget(mUploadBar);

    // connections
    connect(mTimer, &QTimer::timeout, this, &DashboardPage::updateCpuBar);
    connect(mTimer, &QTimer::timeout, this, &DashboardPage::updateMemoryBar);
    connect(mTimer, &QTimer::timeout, this, &DashboardPage::updateNetworkBar);
    if(sysInfo.sucessGPUInfo){
        connect(mTimer, &QTimer::timeout, this, &DashboardPage::updateGPUUtilisationBar);
        connect(mTimer, &QTimer::timeout, this, &DashboardPage::updateGPUMemoryBar);
    }


    QTimer *timerDisk = new QTimer(this);
    connect(timerDisk, &QTimer::timeout, this, &DashboardPage::updateDiskBar);
    timerDisk->start(5 * 1000);

    mTimer->start(1 * 1000);

    // initialization
    updateCpuBar();
    updateMemoryBar();
    updateDiskBar();
    if(sysInfo.sucessGPUInfo){
        updateGPUUtilisationBar();
        updateGPUMemoryBar();
    }

    updateNetworkBar();

    ui->widgetUpdateBar->hide();

    // check update
    checkUpdate();
    connect(this, &DashboardPage::sigShowUpdateBar, ui->widgetUpdateBar, &QWidget::show);
    QList<QWidget*> widgets = {
            mCpuBar, mMemBar, mDiskBar, mDownloadBar, mUploadBar
        };
    if(sysInfo.sucessGPUInfo){
        widgets.insert(3,mGPUMemoryBar);
        widgets.insert(3,mGPUUtilisationBar);
    }


    Utilities::addDropShadow(widgets, 60);


}

void DashboardPage::checkUpdate()
{
    QNetworkAccessManager * nam = new QNetworkAccessManager(this);
    const QNetworkRequest updateCheckRequest(QUrl("https://api.github.com/repos/oguzhaninan/Stacer/releases/latest"));
    connect(nam,&QNetworkAccessManager::finished,this,[this](QNetworkReply * reply){
        if(reply->error()==QNetworkReply::NoError)
        {
            const QString requestResult= reply->readAll();
            const QJsonDocument result = QJsonDocument::fromJson(requestResult.toUtf8());
            const QRegExp ex("([0-9].[0-9].[0-9])");
            ex.indexIn(result.object().value("tag_name").toString());

            if (ex.matchedLength() > 0)
            {
                const QString version = ex.cap();

                if (qApp->applicationVersion() != version) {
                    emit sigShowUpdateBar();
                }
            }
        }

    });
    nam->get(updateCheckRequest);
}

void DashboardPage::on_btnDownloadUpdate_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/oguzhaninan/Stacer/releases/latest"));
}

void DashboardPage::systemInformationInit()
{
    // get system information
    //SystemInfo sysInfo;

    QStringList infos;
    infos
        << tr("Hostname: %1").arg(sysInfo.getHostname())
        << tr("Platform: %1").arg(sysInfo.getPlatform())
        << tr("Distribution: %1").arg(sysInfo.getDistribution())
        << tr("Kernel Release: %1").arg(sysInfo.getKernel())
        << tr("CPU Model: %1").arg(sysInfo.getCpuModel())
        << tr("CPU Core: %1").arg(sysInfo.getCpuCore())
        << tr("CPU Speed: %1 GHz").arg(im->getCpuClock()/1000.0,2,'f',1);

    if(sysInfo.sucessGPUInfo){
    infos
        << tr("GPU Driver Version: %1").arg(sysInfo.getGPUDriverVersion())
        << tr("GPU Accelerator: %1").arg(sysInfo.getGPUAcceleratorVersion())
        << tr("GPU Number Nodes: %1").arg(sysInfo.getGPUCoreString());
    }

    QStringListModel *systemInfoModel = new QStringListModel(infos,ui->listViewSystemInfo);
    const auto oldModel = ui->listViewSystemInfo->selectionModel();
    delete  oldModel;
    ui->listViewSystemInfo->setModel(systemInfoModel);
}

void DashboardPage::updateCpuBar()
{

    QList<int> list_percents_cpu_utilisation = im->getCpuPercents();

    double cpuUsedPercent = std::accumulate(list_percents_cpu_utilisation.begin(),list_percents_cpu_utilisation.end(),0.)/list_percents_cpu_utilisation.size();
    double cpuCurrentClockGHz = im->getCpuClock()/1000.0;

    // alert message
    int cpuAlerPercent = mSettingManager->getCpuAlertPercent();
    if (cpuAlerPercent > 0) {
        static bool isShow = true;
        if (cpuUsedPercent > cpuAlerPercent && isShow) {
            AppManager::ins()->getTrayIcon()->showMessage(tr("High CPU Usage"),
                                                          tr("The amount of CPU used is over %1%.").arg(cpuAlerPercent),
                                                          QSystemTrayIcon::Warning);
            isShow = false;
        } else if (cpuUsedPercent < cpuAlerPercent) {
            isShow = true;
        }
    }

    mCpuBar->setValue(cpuUsedPercent, QString("%1 GHz -- %2%").arg(cpuCurrentClockGHz, 0, 'f', 2).arg(cpuUsedPercent, 0, 'f', 2));
}

void DashboardPage::updateMemoryBar()
{
    im->updateMemoryInfo();

    int memUsedPercent = 0;
    if (im->getMemTotal()) {
        memUsedPercent = ((double)im->getMemUsed() / (double)im->getMemTotal()) * 100.0;
    }

    QString f_memUsed  = FormatUtil::formatBytes(im->getMemUsed());
    QString f_memTotal = FormatUtil::formatBytes(im->getMemTotal());

    // alert message
    int memoryAlertPercent = mSettingManager->getMemoryAlertPercent();
    if (memoryAlertPercent > 0) {
        static bool isShow = true;
        if (memUsedPercent > memoryAlertPercent && isShow) {
            AppManager::ins()->getTrayIcon()->showMessage(tr("High Memory Usage"),
                                                          tr("The amount of memory used is over %1%.").arg(memoryAlertPercent),
                                                          QSystemTrayIcon::Warning);
            isShow = false;
        } else if (memUsedPercent < memoryAlertPercent) {
            isShow = true;
        }
    }

    mMemBar->setValue(memUsedPercent, QString("%1 / %2")
                     .arg(f_memUsed)
                     .arg(f_memTotal));
}

void DashboardPage::updateGPUMemoryBar()
{

    std::vector<double> gpu_memory_used = im->getGPUMemoryUsage();
    std::vector<double> gpu_memory_total = sysInfo.getGPUTotalMemory();


    double sum_total_memory = std::accumulate(gpu_memory_total.begin(), gpu_memory_total.end(), decltype(gpu_memory_total)::value_type(0))/1e3;
    double sum_used_memory = std::accumulate(gpu_memory_used.begin(), gpu_memory_used.end(), decltype(gpu_memory_used)::value_type(0))/1e3;


    // // alert message
    // int cpuAlerPercent = mSettingManager->getCpuAlertPercent();
    // if (cpuAlerPercent > 0) {
    //     static bool isShow = true;
    //     if (cpuUsedPercent > cpuAlerPercent && isShow) {
    //         AppManager::ins()->getTrayIcon()->showMessage(tr("High CPU Usage"),
    //                                                       tr("The amount of CPU used is over %1%.").arg(cpuAlerPercent),
    //                                                       QSystemTrayIcon::Warning);
    //         isShow = false;
    //     } else if (cpuUsedPercent < cpuAlerPercent) {
    //         isShow = true;
    //     }
    // }

    mGPUMemoryBar->setValue(sum_used_memory/sum_total_memory*100, QString("%1 GiB/%2 GiB").arg(sum_used_memory, 0, 'f', 1).arg(sum_total_memory, 0, 'f', 1));
}

void DashboardPage::updateGPUUtilisationBar()
{
    std::vector<double> gpu_utilisation = im->getGPUUsage();

    double sum_utilisation = std::accumulate(gpu_utilisation.begin(), gpu_utilisation.end(), decltype(gpu_utilisation)::value_type(0));

    size_t number_gpu = gpu_utilisation.size();

    // // alert message
    // int cpuAlerPercent = mSettingManager->getCpuAlertPercent();
    // if (cpuAlerPercent > 0) {
    //     static bool isShow = true;
    //     if (cpuUsedPercent > cpuAlerPercent && isShow) {
    //         AppManager::ins()->getTrayIcon()->showMessage(tr("High CPU Usage"),
    //                                                       tr("The amount of CPU used is over %1%.").arg(cpuAlerPercent),
    //                                                       QSystemTrayIcon::Warning);
    //         isShow = false;
    //     } else if (cpuUsedPercent < cpuAlerPercent) {
    //         isShow = true;
    //     }
    // }

    mGPUUtilisationBar->setValue(sum_utilisation/number_gpu, QString(" %1 Percents / %2 ").arg(sum_utilisation, 3, 'f', 0).arg(number_gpu*100., 3, 'f', 0));
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

        if (! disk) {
            for (Disk *d: im->getDisks())
                if (d->name.trimmed() == QStorageInfo::root().displayName().trimmed())
                    disk = d;
            if (! disk)
                disk = im->getDisks().at(0);
        }

        int diskPercent = 0;
        if (disk->size > 0) {
            diskPercent = ((double) disk->used / (double) disk->size) * 100.0;
        }

        // alert message
        int diskAlertPercent = mSettingManager->getDiskAlertPercent();
        if (diskAlertPercent > 0) {
            static bool isShow = true;
            if (diskPercent > diskAlertPercent && isShow) {
                AppManager::ins()->getTrayIcon()->showMessage(tr("High Disk Usage"),
                                                              tr("The amount of disk used is over %1%.").arg(diskAlertPercent),
                                                              QSystemTrayIcon::Warning);
                isShow = false;
            } else if (diskPercent < diskAlertPercent) {
                isShow = true;
            }
        }

        QString sizeText = FormatUtil::formatBytes(disk->size);
        QString usedText = FormatUtil::formatBytes(disk->used);

        mDiskBar->setValue(diskPercent, QString("%1 / %2")
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

    mDownloadBar->setValue(downPercent,
                          QString("%1/s").arg(downText),
                          tr("Total: %1").arg(FormatUtil::formatBytes(RXbytes)));

    mUploadBar->setValue(upPercent,
                        QString("%1/s").arg(upText),
                        tr("Total: %1").arg(FormatUtil::formatBytes(TXbytes)));

    max_RXbytes = qMax(max_RXbytes, d_RXbytes);
    max_TXbytes = qMax(max_TXbytes, d_TXbytes);

    l_RXbytes = RXbytes;
    l_TXbytes = TXbytes;
}

