#ifndef SLIDINGSTACKEDWIDGET_H
#define SLIDINGSTACKEDWIDGET_H

#include <QStackedWidget>

#include <QtGui>
#include <QWidget>
#include <QDebug>
#include <QEasingCurve>

class SlidingStackedWidget : public QStackedWidget
{
    Q_OBJECT

public:
    // This enumeration is used to define the animation direction
    enum t_direction {
        LEFT2RIGHT,
        RIGHT2LEFT,
        TOP2BOTTOM,
        BOTTOM2TOP,
        AUTOMATIC
    };

    SlidingStackedWidget(QWidget *parent);
    ~SlidingStackedWidget(void);

public slots:
    void setSpeed(int speed); // animation duration in milliseconds
    void setAnimation(const QEasingCurve::Type animationtype); // check out the QEasingCurve documentation for different styles
    void setVerticalMode(bool vertical = true);

    void slideInNext();
    void slideInPrev();
    void slideInIdx(int idx, t_direction direction = AUTOMATIC);

signals:
    void animationFinished();

private slots:
    void animationDoneSlot();

private:
    void slideInWgt(QWidget *widget, t_direction direction = AUTOMATIC);

    enum QEasingCurve::Type animationtype;
    int speed;
    bool vertical;
    int now;
    int next;
    QPoint pnow;
    bool active;
};

#endif // SLIDINGSTACKEDWIDGET_H
