#ifndef UNINSTALLERPAGE_H
#define UNINSTALLERPAGE_H

#include <QWidget>
#include <QListWidgetItem>
#include <QMovie>
#include <QtConcurrent>

#include "Managers/tool_manager.h"
#include "Managers/app_manager.h"

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
private slots:
    void init();
    void setAppCount();

    void on_packageSearch_textChanged(const QString &val);
    void on_uninstallBtn_clicked();
    QStringList getSelectedPackages();
    void loadPackages();

private:
    Ui::UninstallerPage *ui;

private:
    ToolManager *tm;
    QIcon icon;
    QMovie *loadingMovie;
};

#endif // UNINSTALLERPAGE_H
