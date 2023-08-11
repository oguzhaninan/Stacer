#ifndef STARTUP_APP_EDIT_H
#define STARTUP_APP_EDIT_H

#include "Managers/app_manager.h"

#include <QRegularExpression>
#include <QDialog>

#define NAME_REG QRegularExpression("^Name=.*")
#define COMMENT_REG QRegularExpression("^Comment=.*")
#define EXEC_REG QRegularExpression("^Exec=.*")
#define GNOME_ENABLED_REG QRegularExpression("^X-GNOME-Autostart-enabled=.*")
#define HIDDEN_REG QRegularExpression("^Hidden=.*")

namespace Ui {
    class StartupAppEdit;
}

class StartupAppEdit : public QDialog
{
    Q_OBJECT

public:
    explicit StartupAppEdit(QWidget *parent = nullptr);
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
    void changeDesktopValue(QStringList &lines, const QRegularExpression &reg, const QString &text);

private:
    Ui::StartupAppEdit *ui;

private:
    QString mNewAppTemplate;
    QString mAutostartPath;
};

#endif // STARTUP_APP_EDIT_H
