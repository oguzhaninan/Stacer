#include "services_page.h"
#include "ui_services_page.h"
#include "service_item.h"

#include "Managers/tool_manager.h"

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
    loadServices();

    if(ui->serviceListWidget->count()) {
        ui->notFoundWidget->hide();
    }
    else { // list widget is empty show not found
        ui->notFoundWidget->show();
        ui->serviceListWidget->hide();
    }

    ui->cmbRunningStatus->addItems({ tr("Running Status"), tr("Running"), tr("Not Running") });
    ui->cmbStartupStatus->addItems({ tr("Startup Status"), tr("Enabled"), tr("Disabled") });
}

void ServicesPage::loadServices()
{
    ui->serviceListWidget->clear();

    QList<Service> services = ToolManager::ins()->getServices();

    int runningIndex = ui->cmbRunningStatus->currentIndex();
    int startupIndex = ui->cmbStartupStatus->currentIndex();

    bool runningStatus = runningIndex == 1;
    bool startupStatus = startupIndex == 1;

    for (const Service s : services) {
        bool runningFilter = runningIndex != 0 ? s.active == runningStatus : true;
        bool startupFilter = startupIndex != 0 ? s.status == startupStatus : true;

        if (runningFilter && startupFilter) {
            ServiceItem *service = new ServiceItem(s.name, s.description, s.status, s.active, this);

            QListWidgetItem *item = new QListWidgetItem(ui->serviceListWidget);

            item->setSizeHint(service->sizeHint());

            ui->serviceListWidget->setItemWidget(item, service);
        }
    }

    setServiceCount();
}

void ServicesPage::setServiceCount()
{
    ui->servicesTitle->setText(tr("System Services (%1)")
                               .arg(ui->serviceListWidget->count()));
}

void ServicesPage::on_cmbRunningStatus_currentIndexChanged(int index)
{
    loadServices();
}

void ServicesPage::on_cmbStartupStatus_currentIndexChanged(int index)
{
    loadServices();
}
