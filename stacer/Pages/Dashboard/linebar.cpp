#include "linebar.h"
#include "ui_linebar.h"

LineBar::~LineBar()
{
    delete ui;
}

LineBar::LineBar(const QString &title, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LineBar)
{
    ui->setupUi(this);

    ui->lineChartTitle->setText(title);
}

void LineBar::setValue(const int &value, const QString &text, const QString &totalText)
{
    ui->lineChartProgress->setValue(value);
    ui->lineChartValue->setText(text);
    ui->lineChartTotal->setText(totalText);
}
