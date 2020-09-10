#include "dashboard_page.h"
#include "ui_dashboard_page.h"


#include "utilities.h"

#include <algorithm>
#include <numeric>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "pugixml.hpp"

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <numeric>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

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
    // Circle bars
    ui->circleBarsLayout->addWidget(mCpuBar);
    ui->circleBarsLayout->addWidget(mMemBar);
    ui->circleBarsLayout->addWidget(mGPUUtilisationBar);
    ui->circleBarsLayout->addWidget(mGPUMemoryBar);
    ui->circleBarsLayout->addWidget(mDiskBar);

    // line bars
    ui->lineBarsLayout->addWidget(mDownloadBar);
    ui->lineBarsLayout->addWidget(mUploadBar);

    // connections
    connect(mTimer, &QTimer::timeout, this, &DashboardPage::updateCpuBar);
    connect(mTimer, &QTimer::timeout, this, &DashboardPage::updateMemoryBar);
    connect(mTimer, &QTimer::timeout, this, &DashboardPage::updateNetworkBar);
    connect(mTimer, &QTimer::timeout, this, &DashboardPage::updateGPUUtilisationBar);
    connect(mTimer, &QTimer::timeout, this, &DashboardPage::updateGPUMemoryBar);

    QTimer *timerDisk = new QTimer(this);
    connect(timerDisk, &QTimer::timeout, this, &DashboardPage::updateDiskBar);
    timerDisk->start(5 * 1000);

    mTimer->start(1 * 1000);

    // initialization
    updateCpuBar();
    updateMemoryBar();
    updateDiskBar();
    updateGPUUtilisationBar();
    updateGPUMemoryBar();
    updateNetworkBar();

    ui->widgetUpdateBar->hide();

    // check update
    checkUpdate();
    connect(this, &DashboardPage::sigShowUpdateBar, ui->widgetUpdateBar, &QWidget::show);

    QList<QWidget*> widgets = {
        mCpuBar, mMemBar, mDiskBar, mGPUMemoryBar, mDownloadBar, mUploadBar
    };

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

bool find_driver_version(pugi::xml_node node)
{
    return strcmp(node.name(), "driver_version") == 0;
}

bool find_cuda_version(pugi::xml_node node)
{
    return strcmp(node.name(), "cuda_version") == 0;
}

void DashboardPage::systemInformationInit()
{
    // get system information
    std::string xml_return = exec("nvidia-smi -q -x"); // get the xml query from nvidia-smi
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_return.c_str());

    std::string GPU_driver = doc.first_child().find_node(find_driver_version).first_child().value();
    std::string cuda_driver = doc.first_child().find_node(find_cuda_version).first_child().value();
    
    SystemInfo sysInfo;

    QStringList infos;
    infos
        << tr("Hostname: %1").arg(sysInfo.getHostname())
        << tr("Platform: %1").arg(sysInfo.getPlatform())
        << tr("Distribution: %1").arg(sysInfo.getDistribution())
        << tr("Kernel Release: %1").arg(sysInfo.getKernel())
        << tr("CPU Model: %1").arg(sysInfo.getCpuModel())
        << tr("CPU Core: %1").arg(sysInfo.getCpuCore())
        << tr("CPU Speed: %1 GHz").arg(im->getCpuClock()/1000.0,2,'f',1)
        << tr("GPU version: %1").arg(QString::fromStdString(GPU_driver))
        << tr("GPU CUDA: %1").arg(QString::fromStdString(cuda_driver));


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

bool find_gpu(pugi::xml_node node)
{
    return strcmp(node.name(), "gpu") == 0;
}

bool find_memory(pugi::xml_node node)
{
    return strcmp(node.name(), "fb_memory_usage") == 0;
}

bool find_memory_tol(pugi::xml_node node)
{
    return strcmp(node.name(), "total") == 0;
}

bool find_memory_used(pugi::xml_node node)
{
    return strcmp(node.name(), "used") == 0;
}

void DashboardPage::updateGPUMemoryBar()
{
    int cpuUsedPercent = im->getCpuPercents().at(0);
    double cpuCurrentClockGHz = im->getCpuClock()/1000.0;

    std::string xml_return = exec("nvidia-smi -q -x"); // get the xml query from nvidia-smi
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_return.c_str());//"<mesh name='sphere'><bounds>0 0 1 1</bounds></mesh>");//xml_return.c_str());//xml_return, size_t(xml_return));

    std::string local_string_gpu_memory_total;
    std::string local_string_gpu_memory_used;
    std::string local_string_gpu_utilisation;

    std::vector<double> gpu_utilisation;
    std::vector<double> gpu_memory_used;
    std::vector<double> gpu_memory_total;

    pugi::xml_node node_inside;

    for (pugi::xml_node gpu: doc.first_child().children("gpu"))
    {

        //local_string_gpu_memory_total
        local_string_gpu_memory_total = gpu.find_node(find_memory).find_node(find_memory_tol).first_child().value();
        gpu_memory_total.push_back(std::stod(local_string_gpu_memory_total.erase(local_string_gpu_memory_total.find(" MiB"))));

        local_string_gpu_memory_used = gpu.find_node(find_memory).find_node(find_memory_used).first_child().value();
        gpu_memory_used.push_back(std::stod(local_string_gpu_memory_used.erase(local_string_gpu_memory_used.find(" MiB"))));

    }

    double sum_total_memory = std::accumulate(gpu_memory_total.begin(), gpu_memory_total.end(), decltype(gpu_memory_total)::value_type(0))/1e3;
    double sum_used_memory = std::accumulate(gpu_memory_used.begin(), gpu_memory_used.end(), decltype(gpu_memory_used)::value_type(0))/1e3;



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

    mGPUMemoryBar->setValue(sum_used_memory/sum_total_memory*100, QString("%1 GiB/%2 GiB").arg(sum_used_memory, 0, 'f', 1).arg(sum_total_memory, 0, 'f', 1));
}

bool find_gpu_utilisation(pugi::xml_node node)
{
    return strcmp(node.name(), "utilization") == 0;
}


bool find_gpu_used(pugi::xml_node node)
{
    return strcmp(node.name(), "gpu_util") == 0;
}

void DashboardPage::updateGPUUtilisationBar()
{
    int cpuUsedPercent = im->getCpuPercents().at(0);
    double cpuCurrentClockGHz = im->getCpuClock()/1000.0;

    std::string xml_return = exec("nvidia-smi -q -x"); // get the xml query from nvidia-smi
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_return.c_str());//"<mesh name='sphere'><bounds>0 0 1 1</bounds></mesh>");//xml_return.c_str());//xml_return, size_t(xml_return));

    std::string local_string_gpu_utilisation;

    std::vector<double> gpu_utilisation;

    pugi::xml_node node_inside;

    for (pugi::xml_node gpu: doc.first_child().children("gpu"))
    {

        //local_string_gpu_memory_total
        local_string_gpu_utilisation = gpu.find_node(find_gpu_utilisation).find_node(find_gpu_used).first_child().value();
        gpu_utilisation.push_back(std::stod(local_string_gpu_utilisation.erase(local_string_gpu_utilisation.find(" %"))));

    }

    double sum_utilisation = std::accumulate(gpu_utilisation.begin(), gpu_utilisation.end(), decltype(gpu_utilisation)::value_type(0));

    size_t number_gpu = gpu_utilisation.size();

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

