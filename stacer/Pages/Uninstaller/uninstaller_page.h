#ifndef UNINSTALLERPAGE_H
#define UNINSTALLERPAGE_H

#include <QWidget>
#include <QListWidgetItem>
#include <QtConcurrent>

#include "Managers/tool_manager.h"
#include "Managers/app_manager.h"
#include "signal_mapper.h"

namespace Ui {
    class UninstallerPage;
}

class UninstallerPage : public QWidget
{
    Q_OBJECT

public:
    explicit UninstallerPage(QWidget *parent = 0);
    ~UninstallerPage();

public slots:
    void uninstallStarted();

private:
    void init();

private slots:
    void setAppCount();
    void on_txtPackageSearch_textChanged(const QString &val);
    void on_btnUninstall_clicked();
    QStringList getSelectedPackages();
    QStringList getSelectedSnapPackages();
    void loadPackages();
    void loadSnapPackages();
    void on_btnSystemPackages_clicked();
    void on_btnSnapPackages_clicked();

    void on_listWidgetSnapPackages_itemClicked(QListWidgetItem *item);

    void on_listWidgetPackages_itemClicked(QListWidgetItem *item);

private:
    Ui::UninstallerPage *ui;

    ToolManager *tm;
};

#endif // UNINSTALLERPAGE_H
