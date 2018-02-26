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
    void diskCmbChanged(const int &index);
    void on_checkAutostart_clicked(bool checked);
    void on_btnDonate_clicked();
    void cmbHomePageChanged(const QString text);

private:
    Ui::SettingsPage *ui;

private:
    AppManager *apm;

    QString startupAppPath;
};

#endif // SETTINGS_PAGE_H
