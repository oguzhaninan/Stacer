#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <QDialog>

namespace Ui {
class Feedback;
}

class Feedback : public QDialog
{
    Q_OBJECT

public:
    explicit Feedback(QWidget *parent = 0);
    ~Feedback();

signals:
    void setErrorMessageS(const QString &msg);
    void clearInputsS();
    void disableElementsS(const bool status);

private slots:
    void setErrorMessage(const QString &msg);
    void on_btnSend_clicked();
    void clearInputs();
    void disableElements(const bool status);

private:
    void init();

private:
    Ui::Feedback *ui;

    QString header;
    QString feedbackUrl;

    QRegExp mailRegex;
};

#endif // FEEDBACK_H
