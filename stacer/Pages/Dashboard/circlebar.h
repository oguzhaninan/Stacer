#ifndef CIRCLEBAR_H
#define CIRCLEBAR_H

#include <QWidget>
#include <QtCharts>
#include "Managers/app_manager.h"
#include "signal_mapper.h"

namespace Ui {
    class CircleBar;
}

class CircleBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qint64 numPages READ numPages WRITE setNumPages)
    Q_PROPERTY(qint64 curPage  READ curPage  WRITE setCurPage)

public:
    explicit CircleBar(const QString &title, const QStringList &colors, QWidget *parent = 0);
    ~CircleBar();

    qint64 numPages() const;
    qint64 curPage() const;
    void setNumPages(const qint64& pages);
    void setCurPage(const qint64& page);

    CircleBar *current() const;

public slots:
    void setValue(const int &value, const QString &valueText);

private slots:
    void init();

signals:
    void change_page(const qint64 oldpg, qint64 newpg);

private:
    Ui::CircleBar *ui;

private:
    QStringList mColors;

    QChart *mChart;
    QChartView *mChartView;
    QPieSeries *mSeries;

    qint64 mNumPages;
    qint64 mCurPage;

    QObjectList mPageList;

protected:
    virtual void mousePressEvent(QMouseEvent *event);
};

#endif // CIRCLEBAR_H
