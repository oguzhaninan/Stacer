#ifndef STARTUPAPPSPAGE_H
#define STARTUPAPPSPAGE_H

#include <QWidget>
#include <QDebug>
#include <QSharedPointer>
#include <QAbstractItemModel>
#include <QFileSystemWatcher>

#include "startup_app.h"
#include "startup_app_edit.h"

#include "Utils/file_util.h"

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
    void openStartupAppEdit(const QString filePath = QString());
    void setAppCount();

private:
    Ui::StartupAppsPage *ui;

private:
    QSharedPointer<StartupAppEdit> mStartupAppEdit;

    QFileSystemWatcher mFileSystemWatcher;
    QString mAutostartPath;

    bool checkIfDisabled(const QString& as_path);
};

#endif // STARTUPAPPSPAGE_H
