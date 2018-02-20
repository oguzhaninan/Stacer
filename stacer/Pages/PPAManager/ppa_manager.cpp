#include "ppa_manager.h"
#include "ui_ppa_manager.h"

PPAManager::PPAManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PPAManager)
{
    ui->setupUi(this);
}

PPAManager::~PPAManager()
{
    delete ui;
}
