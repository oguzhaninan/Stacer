#include "search_page.h"
#include "ui_search_page.h"

SearchPage::SearchPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchPage)
{
    ui->setupUi(this);
}

SearchPage::~SearchPage()
{
    delete ui;
}
