#ifndef SERVICESPAGE_H
#define SERVICESPAGE_H

#include <QWidget>

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
};

#endif // SERVICESPAGE_H
