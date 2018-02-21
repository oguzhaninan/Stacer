#include "apt_source_repository_item.h"
#include "ui_apt_source_repository_item.h"
#include "utilities.h"
#include "Utils/command_util.h"
#include <QDebug>

APTSourceRepositoryItem::~APTSourceRepositoryItem()
{
    delete ui;
}

APTSourceRepositoryItem::APTSourceRepositoryItem(APTSourcePtr aptSource, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::APTSourceRepositoryItem),
    mAptSource(aptSource)
{
    init();
}

void APTSourceRepositoryItem::init()
{
    ui->setupUi(this);

    Utilities::addDropShadow(this, 50);

    ui->aptSourceCheck->setChecked(mAptSource->isActive);

    if (mAptSource->isSource) {
        ui->aptSourceRepositoryName->setText(
                    tr("%1 (Source Code)").arg(mAptSource->source));
    } else {
        ui->aptSourceRepositoryName->setText(mAptSource->source);
    }
}

void APTSourceRepositoryItem::on_deleteAptSourceBtn_clicked()
{
    QStringList args = { "-r", "-y", mAptSource->source };
    try {
        CommandUtil::sudoExec("add-apt-repository", args);
    } catch(QString &ex) {
        qDebug() << ex;
    }
}
