#ifndef SEARCH_PAGE_H
#define SEARCH_PAGE_H

#include <QWidget>
#include <QFileDialog>

namespace Ui {
class SearchPage;
}

class SearchPage : public QWidget
{
    Q_OBJECT

public:
    explicit SearchPage(QWidget *parent = 0);
    ~SearchPage();

private slots:
    void on_btnBrowseSearchDir_clicked();
    void init();

    void on_btnAdvancePaneToggle_clicked();

private:
    Ui::SearchPage *ui;
};

#endif // SEARCH_PAGE_H
