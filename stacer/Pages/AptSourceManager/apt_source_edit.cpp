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
    ui->errorMsg->hide();
}

void APTSourceEdit::show()
{
    clearElements();

    // example 'deb [arch=amd64 lang=en] http://packages.microsoft.com/repos/vscode stable main'
    QString aptSource = selectedAptSource->source;
    QString sourceType, options, URI, distribution, components;

    if (aptSource.contains('[')) {
        int pos1 = aptSource.indexOf('['), pos2 = aptSource.indexOf(']');

        options = aptSource.mid(pos1, pos2-pos1+1);
    }

    aptSource.replace(options, ""); // delete options section
    QStringList sourceColumns = aptSource.split(QRegExp("\\s+"));

    // get values
    sourceType = sourceColumns.takeAt(0);
    URI = sourceColumns.takeAt(0);
    distribution = sourceColumns.takeAt(0);
    components = sourceColumns.join(' ');

    // set values to elements
    ui->radioBinary->setChecked(sourceType == "deb");
    ui->radioSource->setChecked(sourceType == "deb-src");
    ui->txtOptions->setText(options);
    ui->txtUri->setText(URI);
    ui->txtDistribution->setText(distribution);
    ui->txtComponents->setText(components);

    QDialog::show();
}

void APTSourceEdit::clearElements()
{
    ui->errorMsg->hide();
    ui->txtOptions->clear();
    ui->txtUri->clear();
    ui->txtDistribution->clear();
    ui->txtComponents->clear();
}

void APTSourceEdit::on_saveBtn_clicked()
{
    if (! ui->txtUri->text().isEmpty() && ! ui->txtDistribution->text().isEmpty())
    {
        QString sourceType = ui->radioBinary->isChecked() ? "deb" : "deb-src";
        QString updatedAptSource = QString("%1 %2 %3 %4 %5")
                .arg(sourceType)
                .arg(ui->txtOptions->text())
                .arg(ui->txtUri->text())
                .arg(ui->txtDistribution->text())
                .arg(ui->txtComponents->text());

        ToolManager::ins()->changeSource(selectedAptSource, updatedAptSource);

        close();
    } else {
        ui->errorMsg->show();
    }
}
