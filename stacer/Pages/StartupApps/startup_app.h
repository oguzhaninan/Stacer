#ifndef STARTUP_APP_H
#define STARTUP_APP_H

#include <QWidget>
#include <QDebug>
#include <Utils/file_util.h>

#include "startup_app_edit.h"
#include "startup_apps_page.h"

namespace Ui {
    class StartupApp;
}

class StartupApp : public QWidget
{
    Q_OBJECT

public:
    explicit StartupApp(const QString &appName, bool enabled, const QString &filePath, QWidget *parent = 0);
    ~StartupApp();

    QString getAppName() const;
    void setAppName(const QString &value);

    bool getEnabled() const;
    void setEnabled(bool value);

    QString getFilePath() const;
    void setFilePath(const QString &value);

private slots:
    void on_startupCheck_clicked(bool);
    void on_deleteAppBtn_clicked();
    void on_editAppBtn_clicked();

signals:
    void deleteApp();

private:
    Ui::StartupApp *ui;

private:
    StartupAppEdit *startupAppEdit;
    QString appName;
    QString appComment;
    bool enabled;
    QString filePath;
};

#endif // STARTUP_APP_H
