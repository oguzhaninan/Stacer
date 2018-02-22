#ifndef APT_SOURCE_EDIT_H
#define APT_SOURCE_EDIT_H

#include <QDialog>
#include "Managers/tool_manager.h"

namespace Ui {
class APTSourceEdit;
}

class APTSourceEdit : public QDialog
{
    Q_OBJECT

public:
    explicit APTSourceEdit(QWidget *parent = 0);
    ~APTSourceEdit();

public:
    static APTSourcePtr selectedAptSource;
    void show();

private slots:
    void clearElements();
    void on_saveBtn_clicked();
    void on_btnCancel_clicked();

private:
    void init();

private:
    Ui::APTSourceEdit *ui;
};

#endif // APT_SOURCE_EDIT_H
