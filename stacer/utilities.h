#ifndef UTILITIES_H
#define UTILITIES_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>

class Utilities
{
public:
    static void addDropShadow(QWidget *widget, int alpha, int blur = 15)
    {
        QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(widget);
        effect->setBlurRadius(blur);
        effect->setColor(QColor(0, 0, 0, alpha));
        effect->setOffset(0);
        widget->setGraphicsEffect(effect);
    }
};

#endif // UTILITIES_H
