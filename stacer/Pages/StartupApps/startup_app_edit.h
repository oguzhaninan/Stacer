#ifndef STARTUP_APP_EDIT_H
#define STARTUP_APP_EDIT_H

#include <QDialog>
#include <QDesktopWidget>

#include "Managers/app_manager.h"

#define NAME_REG QRegExp("^Name=.*")
#define COMMENT_REG QRegExp("^Comment=.*")
#define EXEC_REG QRegExp("^Exec=.*")
#define GNOME_ENABLED_REG QRegExp("^X-GNOME-Autostart-enabled=.*")
#define HIDDEN_REG QRegExp("^Hidden=.*")

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
    void startupAppAdded();

public slots:
    void show();

private slots:
    void init();
    bool isValid();
    void on_btnSave_clicked();
    void changeDesktopValue(QStringList &lines, const QRegExp &reg, const QString &text);

private:
    Ui::StartupAppEdit *ui;

private:
    QString mNewAppTemplate;
    QString mAutostartPath;
};

#endif // STARTUP_APP_EDIT_H
