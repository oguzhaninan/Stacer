#ifndef SERVICE_ITEM_H
#define SERVICE_ITEM_H

#include <QWidget>
#include <QDebug>
#include "Managers/tool_manager.h"

namespace Ui {
    class ServiceItem;
}

class ServiceItem : public QWidget
{
    Q_OBJECT

public:
    explicit ServiceItem(QString name, bool status, bool active, QWidget *parent = 0);
    ~ServiceItem();

private slots:
    void runningBtn_toggled(bool status);
    void startupBtn_toggled(bool status);

private:
    Ui::ServiceItem *ui;

private:
    QString name;

    ToolManager *tm;
};

#endif // SERVICE_ITEM_H
