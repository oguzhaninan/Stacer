#include "service_item.h"
#include "ui_service_item.h"

ServiceItem::~ServiceItem()
{
    delete ui;
}

ServiceItem::ServiceItem(QString name, bool status, bool active, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServiceItem),
    tm(ToolManager::ins())
{
    ui->setupUi(this);

    ui->serviceName->setText(name);
    ui->runningBtn->setChecked(active);
    ui->startupBtn->setChecked(status);
}

void ServiceItem::on_startupBtn_clicked(bool status)
{
    auto name = ui->serviceName->text();

    tm->changeServiceStatus(name, status);

    ui->startupBtn->setChecked(tm->serviceIsEnabled(name));
}

void ServiceItem::on_runningBtn_clicked(bool status)
{
    auto name = ui->serviceName->text();

    tm->changeServiceActive(name, status);

    ui->runningBtn->setChecked(tm->serviceIsActive(name));
}
