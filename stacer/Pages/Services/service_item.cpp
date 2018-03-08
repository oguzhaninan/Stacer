#include "service_item.h"
#include "ui_service_item.h"
#include "utilities.h"

ServiceItem::~ServiceItem()
{
    delete ui;
}

ServiceItem::ServiceItem(const QString &name,
                         const QString description,
                         const bool status,
                         const bool active,
                         QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServiceItem),
    tm(ToolManager::ins())
{
    ui->setupUi(this);

    ui->lblServiceName->setText(name);
    ui->lblServiceDescription->setText("- " + description);
    ui->checkServiceRunning->setChecked(active);
    ui->checkServiceStartup->setChecked(status);

    ui->lblServiceName->setToolTip(name);
    ui->lblServiceDescription->setToolTip(description);    

    Utilities::addDropShadow(this, 30, 10);
}

void ServiceItem::on_checkServiceStartup_clicked(bool status)
{
    QString name = ui->lblServiceName->text();

    tm->changeServiceStatus(name, status);

    ui->checkServiceStartup->setChecked(tm->serviceIsEnabled(name));
}

void ServiceItem::on_checkServiceRunning_clicked(bool status)
{
    QString name = ui->lblServiceName->text();

    tm->changeServiceActive(name, status);

    ui->checkServiceRunning->setChecked(tm->serviceIsActive(name));
}
