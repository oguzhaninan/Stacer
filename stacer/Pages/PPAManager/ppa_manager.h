#ifndef PPA_MANAGER_H
#define PPA_MANAGER_H

#include <QWidget>

namespace Ui {
class PPAManager;
}

class PPAManager : public QWidget
{
    Q_OBJECT

public:
    explicit PPAManager(QWidget *parent = 0);
    ~PPAManager();

private:
    Ui::PPAManager *ui;
};

#endif // PPA_MANAGER_H
