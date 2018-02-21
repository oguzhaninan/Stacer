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

    QString sourceName;
    // example "deb [arch=amd64] http://packages.microsoft.com/repos/vscode stable main"
    QStringList sourceColumns = mAptSource->source.split(QChar(' '));

    if (sourceColumns[1].startsWith('[')) {
        sourceName = sourceColumns.mid(2).join(' ');
    } else {
        sourceName = sourceColumns.mid(1).join(' ');
    }

    if (mAptSource->isSource) {
        ui->aptSourceRepositoryName->setText(
                    tr("%1 (Source Code)").arg(sourceName));
    } else {
        ui->aptSourceRepositoryName->setText(sourceName);
    }

    ui->aptSourceRepositoryName->setToolTip(ui->aptSourceRepositoryName->text());
}

void APTSourceRepositoryItem::on_deleteAptSourceBtn_clicked()
{
    ToolManager::ins()->removeAPTSource(mAptSource->source);
}

void APTSourceRepositoryItem::on_aptSourceCheck_clicked(bool checked)
{

}
