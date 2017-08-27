#ifndef STARTUPAPPSPAGE_H
#define STARTUPAPPSPAGE_H

#include <QWidget>
#include <QDebug>

#include "startup_app.h"
#include "startup_app_edit.h"

#include <Utils/file_util.h>

namespace Ui {
    class StartupAppsPage;
}

class StartupAppsPage : public QWidget
{
    Q_OBJECT

public:
    explicit StartupAppsPage(QWidget *parent = 0);
    ~StartupAppsPage();

public slots:
    void loadApps();

private slots:
    void init();
    void on_addStartupAppBtn_clicked();
    void setAppCount();

private:
    Ui::StartupAppsPage *ui;

private:
    StartupAppEdit *startupAppEdit;
};

#endif // STARTUPAPPSPAGE_H
