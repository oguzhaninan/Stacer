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
    explicit ServiceItem(const QString &name, const QString description, const bool status, const bool active, QWidget *parent = 0);
    ~ServiceItem();

private slots:
    void on_runningBtn_clicked(bool status);
    void on_startupBtn_clicked(bool status);

private:
    Ui::ServiceItem *ui;

private:
    ToolManager *tm;
};

#endif // SERVICE_ITEM_H
