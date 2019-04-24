#include "host_manage.h"
#include "ui_host_manage.h"

HostManage::~HostManage()
{
    delete ui;
}

HostManage::HostManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HostManage)
{
    ui->setupUi(this);

    init();
}

void HostManage::init()
{
    ui->lblHostTitle->setText(tr("Hosts (%1)").arg(1));


    Utilities::addDropShadow({
        ui->btnDelete, ui->btnNewHost, ui->btnSave, ui->txtAliases, ui->txtFullyQualified,
        ui->txtIP, ui->tableViewHosts
    }, 40);
}
