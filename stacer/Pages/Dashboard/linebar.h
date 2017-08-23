#ifndef LINEBAR_H
#define LINEBAR_H

#include <QWidget>

namespace Ui {
    class LineBar;
}

class LineBar : public QWidget
{
    Q_OBJECT

public:
    explicit LineBar(QString title, QWidget *parent = 0);
    ~LineBar();

public slots:
    void setValue(int value, QString text, QString totalText);
private:
    Ui::LineBar *ui;
};

#endif // LINEBAR_H
