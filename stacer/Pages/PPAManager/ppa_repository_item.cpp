#include "ppa_repository_item.h"
#include "ui_ppa_repository_item.h"
#include "utilities.h"

PPARepositoryItem::~PPARepositoryItem()
{
    delete ui;
}

PPARepositoryItem::PPARepositoryItem(QString repositoryName,
                                     QString repositoryComment,
                                     QString repositoryFilePath,
                                     QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PPARepositoryItem),
    mName(repositoryName),
    mComment(repositoryComment),
    mFilePath(repositoryFilePath)
{
    ui->setupUi(this);

    init();
}

void PPARepositoryItem::init()
{
    Utilities::addDropShadow(this, 50);
    ui->ppaRepositoryName->setText(mName);
}
