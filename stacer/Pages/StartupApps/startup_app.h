#ifndef STARTUP_APP_H
#define STARTUP_APP_H

#include <QWidget>
#include <QDebug>
#include <QSharedPointer>
#include <Utils/file_util.h>

#include "startup_app_edit.h"

namespace Ui {
    class StartupApp;
}

class StartupApp : public QWidget
{
    Q_OBJECT

public:
    explicit StartupApp(const QString &startupAppName, bool enabled, const QString &filePath, QWidget *parent = 0);
    ~StartupApp();

    QString getAppName() const;
    void setAppName(const QString &value);

    bool getEnabled() const;
    void setEnabled(bool value);

    QString getFilePath() const;
    void setFilePath(const QString &value);

private slots:
    void on_checkStartup_clicked(bool);
    void on_btnDeleteStartupApp_clicked();
    void on_btnEditStartupApp_clicked();

signals:
    void deleteAppS();
    void editStartupAppS(const QString filePath);

private:
    Ui::StartupApp *ui;

private:
    QString mStartupAppName;
    QString mAppComment;
    bool mEnabled;
    QString mFilePath;
};

#endif // STARTUP_APP_H
