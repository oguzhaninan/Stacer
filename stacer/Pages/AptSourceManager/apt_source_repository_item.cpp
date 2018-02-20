#include "apt_source_repository_item.h"
#include "ui_apt_source_repository_item.h"
#include "utilities.h"

APTSourceRepositoryItem::~APTSourceRepositoryItem()
{
    delete ui;
}

APTSourceRepositoryItem::APTSourceRepositoryItem(QString repositoryName,
                                     QString repositoryComment,
                                     QString repositoryFilePath,
                                     QWidget *parent) :
    QWidget(parent),
    ui(new Ui::APTSourceRepositoryItem),
    mName(repositoryName),
    mComment(repositoryComment),
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
