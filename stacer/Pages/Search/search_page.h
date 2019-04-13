#ifndef SEARCH_PAGE_H
#define SEARCH_PAGE_H

#include <QWidget>
#include <QFileDialog>
#include "Managers/info_manager.h"

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
    void on_checkFilterTime_clicked(bool checked);
    void on_checkFilterPermissions_clicked(bool checked);
    void on_checkFilterSize_clicked(bool checked);
    void on_checkFilterOwner_clicked(bool checked);

private:
    Ui::SearchPage *ui;
};

#endif // SEARCH_PAGE_H
