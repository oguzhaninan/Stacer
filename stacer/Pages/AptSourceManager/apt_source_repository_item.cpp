#include "apt_source_repository_item.h"
#include "ui_apt_source_repository_item.h"
#include "utilities.h"
#include "Utils/command_util.h"
#include <QDebug>

APTSourceRepositoryItem::~APTSourceRepositoryItem()
{
    delete ui;
}

APTSourceRepositoryItem::APTSourceRepositoryItem(QString repositoryName,
                                     QString repositoryFilePath,
                                     QWidget *parent) :
    QWidget(parent),
    ui(new Ui::APTSourceRepositoryItem),
    mName(repositoryName),
    mFilePath(repositoryFilePath)
{
    ui->setupUi(this);

    init();
}

void APTSourceRepositoryItem::init()
{
    Utilities::addDropShadow(this, 50);

    ui->aptSourceRepositoryName->setText(mName);
}

void APTSourceRepositoryItem::on_deleteAptSourceBtn_clicked()
{
    try {
        CommandUtil::sudoExec("rm", { mFilePath });
        emit aptSourceRepositoryDeleted();
    } catch(QString &ex) {
        qDebug() << ex;
    }
}
