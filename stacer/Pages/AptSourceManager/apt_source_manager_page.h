#ifndef APTSourceManagerPage_PAGE_H
#define APTSourceManagerPage_PAGE_H

#include <QWidget>
#include <QListWidgetItem>
#include <QFileSystemWatcher>

#include "apt_source_repository_item.h"
#include "apt_source_edit.h"

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
    static APTSourcePtr selectedAptSource;

private slots:
    void loadAptSources();
    void on_btnAddAPTSourceRepository_clicked(bool checked);
    void on_aptSourceRepositoryListWidget_itemClicked(QListWidgetItem *item);
    void changeElementsVisible(const bool checked);
    void on_txtSearchAptSource_textChanged(const QString &val);
    void on_btnDeleteAptSource_clicked();
    void on_btnEditAptSource_clicked();
    void on_aptSourceRepositoryListWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_btnCancel_clicked();

    void on_aptSourceRepositoryListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    void init();

private:
    Ui::APTSourceManagerPage *ui;

    QFileSystemWatcher fileSystemWatcher;

    QSharedPointer<APTSourceEdit> aptSourceEditDialog;
};

#endif
