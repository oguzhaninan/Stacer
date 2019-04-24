#ifndef HOST_MANAGE_H
#define HOST_MANAGE_H

#include <QWidget>
#include "utilities.h"

namespace Ui {
class HostManage;
}

class HostManage : public QWidget
{
    Q_OBJECT

public:
    explicit HostManage(QWidget *parent = 0);
    ~HostManage();

private slots:
    void init();

private:
    Ui::HostManage *ui;
};

#endif // HOST_MANAGE_H
