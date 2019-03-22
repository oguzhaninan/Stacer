#ifndef SEARCH_PAGE_H
#define SEARCH_PAGE_H

#include <QWidget>

namespace Ui {
class SearchPage;
}

class SearchPage : public QWidget
{
    Q_OBJECT

public:
    explicit SearchPage(QWidget *parent = 0);
    ~SearchPage();

private:
    Ui::SearchPage *ui;
};

#endif // SEARCH_PAGE_H
