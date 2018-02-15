#ifndef SERVICESPAGE_H
#define SERVICESPAGE_H

#include <QWidget>
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

signals:
    void loadServicesS();

private slots:
    void init();
    void getServices();
    void loadServices();

    void on_cmbRunningStatus_currentIndexChanged(int index);
    void on_cmbStartupStatus_currentIndexChanged(int index);

public slots:
    void setServiceCount();

private:
    Ui::ServicesPage *ui;

    QList<Service> mServices;
};

#endif // SERVICESPAGE_H
