#include "apt_source_manager_page.h"
#include "ui_apt_source_manager_page.h"
#include <QDebug>

APTSourceManagerPage::~APTSourceManagerPage()
{
    delete ui;
}

APTSourceManagerPage::APTSourceManagerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::APTSourceManagerPage)
{
    ui->setupUi(this);

    init();
}

void APTSourceManagerPage::init()
{
    QFile file("/etc/apt/sources.list.d/simonschneegans-ubuntu-testing-xenial.list");

    if (file.open(QIODevice::ReadOnly)) {
        QString content = file.readAll();
        qDebug() << content;

        QListWidgetItem *listItem = new QListWidgetItem(ui->aptSourceRepositoryListWidget);

        APTSourceRepositoryItem *aptSourceItem = new APTSourceRepositoryItem("simonschneegans-ubuntu-testing-xenial",
                                                           "Comment",
                                                           file.fileName());

        ui->aptSourceRepositoryListWidget->setItemWidget(listItem, aptSourceItem);
    }

    ui->notFoundWidget->hide();
}
