#ifndef STARTUP_APP_EDIT_H
#define STARTUP_APP_EDIT_H

#include <QDialog>
#include <QDesktopWidget>

#include "Managers/app_manager.h"

#define NAME_REG QRegExp("(\\W+|^)Name=.*")
#define COMMENT_REG QRegExp("(\\W+|^)Comment=.*")
#define EXEC_REG QRegExp("(\\W+|^)Exec=.*")

namespace Ui {
    class StartupAppEdit;
}

class StartupAppEdit : public QDialog
{
    Q_OBJECT

public:
    explicit StartupAppEdit(QWidget *parent = 0);
    ~StartupAppEdit();

public:
    static QString selectedFilePath;

signals:
    void closeWindow();

public slots:
    void show();

private slots:
    void init();
    bool isValid();
    void on_saveBtn_clicked();

private:
    Ui::StartupAppEdit *ui;

private:
    QString newAppTemplate;
};

#endif // STARTUP_APP_EDIT_H
