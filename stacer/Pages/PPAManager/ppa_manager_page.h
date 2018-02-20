#ifndef PPA_MANAGER_PAGE_H
#define PPA_MANAGER_PAGE_H

#include <QWidget>
#include <QFileSystemWatcher>
#include <QFile>

#include "ppa_repository_item.h"

namespace Ui {
class PPAManagerPage;
}

class PPAManagerPage : public QWidget
{
    Q_OBJECT

public:
    explicit PPAManagerPage(QWidget *parent = 0);
    ~PPAManagerPage();

private:
    void init();

private:
    Ui::PPAManagerPage *ui;


};

#endif // PPA_MANAGER_PAGE_H
