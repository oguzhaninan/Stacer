#include "apt_source_edit.h"
#include "ui_apt_source_edit.h"

#include <QDebug>

APTSourceEdit::~APTSourceEdit()
{
    delete ui;
}

APTSourcePtr APTSourceEdit::selectedAptSource = nullptr;

APTSourceEdit::APTSourceEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::APTSourceEdit)
{
    ui->setupUi(this);

    init();
}

void APTSourceEdit::init()
{
    ui->lblErrorMsg->hide();
}

void APTSourceEdit::show()
{
    clearElements();

    // example 'deb [arch=amd64 lang=en] http://packages.microsoft.com/repos/vscode stable main'

    // set values to elements
    ui->radioBinary->setChecked(! selectedAptSource->isSource);
    ui->radioSource->setChecked(selectedAptSource->isSource);
    ui->txtOptions->setText(selectedAptSource->options);
    ui->txtUri->setText(selectedAptSource->uri);
    ui->txtDistribution->setText(selectedAptSource->distribution);
    ui->txtComponents->setText(selectedAptSource->components);

    QDialog::show();
}

void APTSourceEdit::clearElements()
{
    ui->lblErrorMsg->hide();
    ui->txtOptions->clear();
    ui->txtUri->clear();
    ui->txtDistribution->clear();
    ui->txtComponents->clear();
}

void APTSourceEdit::on_btnSave_clicked()
{
    if (! ui->txtUri->text().isEmpty() &&
        ! ui->txtDistribution->text().isEmpty())
    {
        QString sourceType = ui->radioBinary->isChecked() ? "deb" : "deb-src";
        QString updatedAptSource = QString("%1 %2 %3 %4 %5")
                .arg(sourceType)
                .arg(ui->txtOptions->text())
                .arg(ui->txtUri->text())
                .arg(ui->txtDistribution->text())
                .arg(ui->txtComponents->text());

        ToolManager::ins()->changeAPTSource(selectedAptSource, updatedAptSource);

        emit saved();

        close();
    } else {
        ui->lblErrorMsg->show();
    }
}

void APTSourceEdit::on_btnCancel_clicked()
{
    close();
}
