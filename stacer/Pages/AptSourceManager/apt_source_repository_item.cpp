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

    ui->checkAptSource->setChecked(mAptSource->isActive);

    QString sourceName = mAptSource->source;
    // example "deb [arch=amd64] http://packages.microsoft.com/repos/vscode stable main"

    if (mAptSource->isSource) {
        ui->lblAptSourceName->setText(
                    tr("%1 (Source Code)").arg(sourceName));
    } else {
        ui->lblAptSourceName->setText(sourceName);
    }

    ui->lblAptSourceName->setToolTip(ui->lblAptSourceName->text());
}

APTSourcePtr APTSourceRepositoryItem::aptSource() const
{
    return mAptSource;
}

void APTSourceRepositoryItem::on_checkAptSource_clicked(bool checked)
{
    ToolManager::ins()->changeStatus(mAptSource, checked);
}
