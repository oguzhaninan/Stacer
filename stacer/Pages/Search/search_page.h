#ifndef SEARCH_PAGE_H
#define SEARCH_PAGE_H

#include <QWidget>
#include <QFileDialog>
#include "Managers/info_manager.h"
#include "utilities.h"

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
    void on_btnSearchAdvance_clicked();
    void initComboboxValues();

private:
    Ui::SearchPage *ui;

    QString mSelectedDirectory;
};

#endif // SEARCH_PAGE_H
