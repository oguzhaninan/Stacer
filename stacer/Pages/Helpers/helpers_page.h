#ifndef HELPERS_PAGE_H
#define HELPERS_PAGE_H

#include <QWidget>
#include "host_manage.h"
#include "utilities.h"

namespace Ui {
class HelpersPage;
}

class HelpersPage : public QWidget
{
    Q_OBJECT

public:
    explicit HelpersPage(QWidget *parent = 0);
    ~HelpersPage();

private slots:
    void on_btnHostManage_clicked();
    void on_btnSshManage_clicked();
    void init();

private:
    Ui::HelpersPage *ui;

    HostManage *widgetHostManage;
};

#endif // HELPERS_PAGE_H
