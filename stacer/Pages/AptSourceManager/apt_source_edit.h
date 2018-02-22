#ifndef APT_SOURCE_EDIT_H
#define APT_SOURCE_EDIT_H

#include <QDialog>

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
    static QString selectedAptSource;
    void show();

private slots:
    void clearElements();
    void on_saveBtn_clicked();

private:
    void init();

private:
    Ui::APTSourceEdit *ui;
};

#endif // APT_SOURCE_EDIT_H
