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
}

void ServicesPage::loadServices()
{
    for (const Service &s : ToolManager::ins()->getServices()) {

        ServiceItem *service = new ServiceItem(s.name, s.status, s.active, this);

        QListWidgetItem *item = new QListWidgetItem(ui->serviceListWidget);

        item->setSizeHint(service->sizeHint());

        ui->serviceListWidget->setItemWidget(item, service);
    }

    setAppCount();
}

void ServicesPage::setAppCount()
{
    ui->servicesTitle->setText(tr("System Services (%1)")
                               .arg(ui->serviceListWidget->count()));
}
