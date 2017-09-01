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
    explicit LineBar(const QString &title, QWidget *parent = 0);
    ~LineBar();

public slots:
    void setValue(const int &value, const QString &text, const QString &totalText);
private:
    Ui::LineBar *ui;
};

#endif // LINEBAR_H
