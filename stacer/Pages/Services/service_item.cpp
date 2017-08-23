#include "service_item.h"
#include "ui_service_item.h"

ServiceItem::~ServiceItem()
{
    delete ui;
}

ServiceItem::ServiceItem(QString name, bool status, bool active, QWidget *parent) :
    QWidget(parent),
    name(name),
    tm(ToolManager::ins()),
    ui(new Ui::ServiceItem)
{
    ui->setupUi(this);

    ui->serviceName->setText(name);
    ui->runningBtn->setChecked(active);
    ui->startupBtn->setChecked(status);

    connect(ui->startupBtn, &QCheckBox::clicked, this, &ServiceItem::startupBtn_toggled);
    connect(ui->runningBtn, &QCheckBox::clicked, this, &ServiceItem::runningBtn_toggled);
}

void ServiceItem::startupBtn_toggled(bool status)
{
    tm->changeServiceStatus(ui->serviceName->text(), status);

    ui->startupBtn->setChecked(tm->serviceIsEnabled(name));
}

void ServiceItem::runningBtn_toggled(bool status)
{
    tm->changeServiceActive(ui->serviceName->text(), status);

    ui->runningBtn->setChecked(tm->serviceIsActive(name));
}
