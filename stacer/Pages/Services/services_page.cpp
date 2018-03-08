#include "services_page.h"
#include "ui_services_page.h"
#include "service_item.h"

#include "utilities.h"
#include <QtConcurrent>

ServicesPage::~ServicesPage()
{
    delete ui;
}

ServicesPage::ServicesPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServicesPage)
{
    ui->setupUi(this);

    init();    
}

void ServicesPage::init()
{
    connect(this, &ServicesPage::loadServicesS, this, &ServicesPage::loadServices);
    QtConcurrent::run(this, &ServicesPage::getServices);

    ui->cmbRunningStatus->addItems({ tr("Running Status"), tr("Running"), tr("Not Running") });
    ui->cmbStartupStatus->addItems({ tr("Startup Status"), tr("Enabled"), tr("Disabled") });

    Utilities::addDropShadow(ui->cmbRunningStatus, 30);
    Utilities::addDropShadow(ui->cmbStartupStatus, 30);
}

void ServicesPage::getServices()
{
    this->mServices = ToolManager::ins()->getServices();
    emit loadServicesS();
}

void ServicesPage::loadServices()
{
    ui->listWidgetServices->clear();

    int runningIndex = ui->cmbRunningStatus->currentIndex();
    int startupIndex = ui->cmbStartupStatus->currentIndex();

    bool runningStatus = runningIndex == 1;
    bool startupStatus = startupIndex == 1;

    for (const Service s : mServices) {
        bool runningFilter = runningIndex != 0 ? s.active == runningStatus : true;
        bool startupFilter = startupIndex != 0 ? s.status == startupStatus : true;

        if (runningFilter && startupFilter) {
            ServiceItem *service = new ServiceItem(s.name, s.description, s.status, s.active);

            QListWidgetItem *item = new QListWidgetItem(ui->listWidgetServices);

            item->setSizeHint(service->sizeHint());

            ui->listWidgetServices->setItemWidget(item, service);
        }
    }

    setServiceCount();

    bool isListEmpty = ui->listWidgetServices->count() == 0;

    ui->listWidgetServices->setVisible(! isListEmpty);
    ui->notFoundWidget->setVisible(isListEmpty);
}

void ServicesPage::setServiceCount()
{
    ui->lblServicesTitle->setText(tr("System Services (%1)")
                               .arg(ui->listWidgetServices->count()));
}

void ServicesPage::on_cmbRunningStatus_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    loadServices();
}

void ServicesPage::on_cmbStartupStatus_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    loadServices();
}
