#ifndef APPEARANCE_SETTINGS_H
#define APPEARANCE_SETTINGS_H

#include <QWidget>

namespace Ui {
class AppearanceSettings;
}

class AppearanceSettings : public QWidget
{
    Q_OBJECT

public:
    explicit AppearanceSettings(QWidget *parent = 0);
    ~AppearanceSettings();

private:
    Ui::AppearanceSettings *ui;
};

#endif // APPEARANCE_SETTINGS_H
