#ifndef UTILITIES_H
#define UTILITIES_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>

class Utilities
{
public:
    static void
    addDropShadow(QWidget *widget, int alpha, int blur = 15)
    {
        addDropShadow(QList<QWidget*>() << widget, alpha, blur);
    }

    static void
    addDropShadow(QList<QWidget *> widgets, int alpha, int blur = 15)
    {
        for (QWidget *widget: widgets) {
            QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(widget);
            effect->setBlurRadius(blur);
            effect->setColor(QColor(0, 0, 0, alpha));
            effect->setOffset(0);
            widget->setGraphicsEffect(effect);
        }
    }

    static QString
    getDesktopValue(const QRegExp &val, const QStringList &lines)
    {
        QStringList filteredList = lines.filter(val);
        if (filteredList.count() > 0) {
            QStringList directive = filteredList.first().trimmed().split("=");
            if (directive.count() > 1) {
                return directive.last().trimmed();
            }
        }
        return QString("");
    }
};

#endif // UTILITIES_H
