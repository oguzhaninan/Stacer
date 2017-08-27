#ifndef SERVICESPAGE_H
#define SERVICESPAGE_H

#include <QWidget>
#include <QDebug>
#include <QtConcurrent>
#include "service_item.h"
#include "Managers/tool_manager.h"

namespace Ui {
    class ServicesPage;
}

class ServicesPage : public QWidget
{
    Q_OBJECT

public:
    explicit ServicesPage(QWidget *parent = 0);
    ~ServicesPage();

private slots:
    void init();
    void loadServices();

public slots:
    void setAppCount();

private:
    Ui::ServicesPage *ui;

private:
    ToolManager *tm;
};

#endif // SERVICESPAGE_H
