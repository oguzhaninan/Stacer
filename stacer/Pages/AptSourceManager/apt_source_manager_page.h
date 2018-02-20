#ifndef APTSourceManagerPage_PAGE_H
#define APTSourceManagerPage_PAGE_H

#include <QWidget>
#include <QFileSystemWatcher>
#include <QFile>

#include "apt_source_repository_item.h"

namespace Ui {
class APTSourceManagerPage;
}

class APTSourceManagerPage : public QWidget
{
    Q_OBJECT

public:
    explicit APTSourceManagerPage(QWidget *parent = 0);
    ~APTSourceManagerPage();

private slots:
    void loadAptSources();
    void on_btnAddAPTSourceRepository_clicked();

private:
    void init();

private:
    Ui::APTSourceManagerPage *ui;

    QFileSystemWatcher fileSystemWatcher;
};

#endif
