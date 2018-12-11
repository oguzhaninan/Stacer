#include "circlebar.h"
#include "ui_circlebar.h"

CircleBar::~CircleBar()
{
    delete ui;
    delete mChart;
}

CircleBar::CircleBar(const QString &title, const QStringList &colors, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CircleBar),
    mColors(colors),
    mChart(new QChart),
    mChartView(new QChartView(mChart)),
    mSeries(new QPieSeries(this)),
    mNumPages(0),
    mCurPage(0)
{
    ui->setupUi(this);

    ui->lblCircleChartTitle->setText(title);

    init();
}

qint64 CircleBar::numPages() const
{
    return mNumPages;
}

qint64 CircleBar::curPage() const
{
    return mCurPage;
}

void CircleBar::setNumPages(const qint64& pages)
{
    qint64 val = pages;
    mNumPages = val;
}
void CircleBar::setCurPage(const qint64& page)
{
    qint64 val = page;
    mCurPage = val;
}

CircleBar *CircleBar::current() const
{
    if (mNumPages < 1)
    {
        return (CircleBar*)this;
    }
    else
    {
        if (mCurPage <= 0)
        {
            return (CircleBar*)this;
        }
        else
        {
            return qobject_cast<CircleBar*>(mPageList[mCurPage]);
        }
    }

    return (CircleBar*)this;
}
void CircleBar::init()
{
    QColor transparent("transparent");

    // series settings
    mSeries->setHoleSize(0.67);
    mSeries->setPieSize(165);
    mSeries->setPieStartAngle(-115);
    mSeries->setPieEndAngle(115);
    mSeries->setLabelsVisible(false);
    mSeries->append("Used", 0);
    mSeries->append("Free", 0);
    mSeries->slices().first()->setBorderColor(transparent);
    mSeries->slices().last()->setBorderColor(transparent);
    QConicalGradient gradient;
    gradient.setAngle(115);
    for (int i = 0; i < mColors.count(); ++i) {
        gradient.setColorAt(i, QColor(mColors.at(i)));
    }
    mSeries->slices().first()->setBrush(gradient);

    // chart settings
    mChart->setBackgroundBrush(QBrush(transparent));
    mChart->setContentsMargins(-20, -20, -20, -65);
    mChart->addSeries(mSeries);
    mChart->legend()->hide();

    // chartview settings
    mChartView->setRenderHint(QPainter::Antialiasing);

    ui->layoutCircleBar->insertWidget(1, mChartView);

    connect(SignalMapper::ins(), &SignalMapper::sigChangedAppTheme, [=] {
        QSettings *styleValues = AppManager::ins()->getStyleValues();
        mChartView->setBackgroundBrush(QColor(styleValues->value("@circleChartBackgroundColor").toString()));
        mSeries->slices().last()->setColor(styleValues->value("@pageContent").toString()); // trail color
    });
}

void CircleBar::setValue(const int &value, const QString &valueText)
{
    mSeries->slices().first()->setValue(value);
    mSeries->slices().last()->setValue(100 - value);

    ui->lblCircleChartValue->setText(valueText);
}

void CircleBar::mousePressEvent(QMouseEvent *event)
{
    qint64 cur = this->curPage();

    switch (event->button())
    {
        case Qt::LeftButton:
            if (this->numPages() < 1)
                return;

            emit change_page(cur, ++cur);
        break;
        case Qt::RightButton:
            if (this->numPages() < 1)
                return;
            if (cur-1 >= 0)
            {
                emit change_page(cur, --cur);
            }
            else { return; }
        break;
        default:
            return;
        break;
    }
}
