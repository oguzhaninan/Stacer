#ifndef WINDOW_MANAGER_SETTINGS_H
#define WINDOW_MANAGER_SETTINGS_H

#include <QWidget>

namespace Ui {
class WindowManagerSettings;
}

class WindowManagerSettings : public QWidget
{
    Q_OBJECT

public:
    explicit WindowManagerSettings(QWidget *parent = 0);
    ~WindowManagerSettings();

private:
    Ui::WindowManagerSettings *ui;
};

#endif // WINDOW_MANAGER_SETTINGS_H
