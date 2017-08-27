#include "linebar.h"
#include "ui_linebar.h"

LineBar::~LineBar()
{
    delete ui;
}

LineBar::LineBar(QString title, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LineBar)
{
    ui->setupUi(this);

    ui->lineChartTitle->setText(title);
}

void LineBar::setValue(int value, QString text, QString totalText)
{
    ui->lineChartProgress->setValue(value);
    ui->lineChartValue->setText(text);
    ui->lineChartTotal->setText(totalText);
}
