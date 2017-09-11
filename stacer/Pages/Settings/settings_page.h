#ifndef SETTINGS_PAGE_H
#define SETTINGS_PAGE_H

#include <QWidget>
#include <QMapIterator>

#include "Managers/app_manager.h"

namespace Ui {
    class SettingsPage;
}

class SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = 0);
    ~SettingsPage();

private slots:
    void init();

    void themesCmbChanged(const int &index);
    void languagesCmbChanged(const int &index);

private:
    Ui::SettingsPage *ui;

private:
    AppManager *apm;
};

#endif // SETTINGS_PAGE_H
