#ifndef APTSourceManagerPage_PAGE_H
#define APTSourceManagerPage_PAGE_H

#include <QWidget>
#include <QListWidgetItem>
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

public:
    static QString selectedAptSource;

private slots:
    void loadAptSources();
    void on_btnAddAPTSourceRepository_clicked(bool checked);
    void on_aptSourceRepositoryListWidget_itemClicked(QListWidgetItem *item);
    void changeElementsVisible(const bool checked);
    void on_txtSearchAptSource_textChanged(const QString &val);
    void on_btnDeleteAptSource_clicked();

private:
    void init();

private:
    Ui::APTSourceManagerPage *ui;

    QFileSystemWatcher fileSystemWatcher;
};

#endif
