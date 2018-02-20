#include "ppa_manager_page.h"
#include "ui_ppa_manager_page.h"
#include <QDebug>

PPAManagerPage::~PPAManagerPage()
{
    delete ui;
}

PPAManagerPage::PPAManagerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PPAManagerPage)
{
    ui->setupUi(this);

    init();
}

void PPAManagerPage::init()
{
    QFile file("/etc/apt/sources.list.d/simonschneegans-ubuntu-testing-xenial.list");

    if (file.open(QIODevice::ReadOnly)) {
        QString content = file.readAll();
        qDebug() << content;

        QListWidgetItem *listItem = new QListWidgetItem(ui->ppaRepositoryListWidget);

        PPARepositoryItem *ppaItem = new PPARepositoryItem("simonschneegans-ubuntu-testing-xenial",
                                                           "Comment",
                                                           file.fileName());

        ui->ppaRepositoryListWidget->setItemWidget(listItem, ppaItem);
    }

    ui->notFoundWidget->hide();
}
