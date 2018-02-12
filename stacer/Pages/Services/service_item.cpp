#include "service_item.h"
#include "ui_service_item.h"
#include "utilities.h"

ServiceItem::~ServiceItem()
{
    delete ui;
}

ServiceItem::ServiceItem(const QString &name, const QString description, const bool status, const bool active, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServiceItem),
    tm(ToolManager::ins())
{
    ui->setupUi(this);

    ui->serviceName->setText(name);
    ui->lblDescription->setText("- " + description);
    ui->runningBtn->setChecked(active);
    ui->startupBtn->setChecked(status);

    ui->serviceName->setToolTip(name);
    ui->lblDescription->setToolTip(description);    

    Utilities::addDropShadow(this, 55);
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
