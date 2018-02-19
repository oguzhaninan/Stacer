#ifndef UNITY_SETTINGS_H
#define UNITY_SETTINGS_H

#include <QWidget>

namespace Ui {
class UnitySettings;
}

class UnitySettings : public QWidget
{
    Q_OBJECT

public:
    explicit UnitySettings(QWidget *parent = 0);
    ~UnitySettings();

private:
    Ui::UnitySettings *ui;
};

#endif // UNITY_SETTINGS_H
