#include "feedback.h"
#include "ui_feedback.h"
#include "Utils/command_util.h"

#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QtConcurrent>

Feedback::~Feedback()
{
    delete ui;
}

Feedback::Feedback(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Feedback),
    mHeader("Content-Type: application/json"),
    mFeedbackUrl("https://stacer-web-api.herokuapp.com/feedback"),
    mMailRegex("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b")
{
    ui->setupUi(this);

    init();
}

void Feedback::init()
{
    mMailRegex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);

    connect(this, &Feedback::clearInputsS,     this, &Feedback::clearInputs);
    connect(this, &Feedback::setErrorMessageS, this, &Feedback::setErrorMessage);
    connect(this, &Feedback::disableElementsS, this, &Feedback::disableElements);
}

void Feedback::on_btnSend_clicked()
{
    QString name = ui->txtName->text();
    QString email = ui->txtEmail->text();
    QString message = ui->txtMessage->toPlainText();

    bool isEmailValid = mMailRegex.match(email).hasMatch();

    if (! isEmailValid) {
        emit setErrorMessageS(tr("Email address is not valid !"));
        return;
    }

    if (message.length() < 5) {
        emit setErrorMessageS(tr("Your message must be at least 5 characters !"));
        return;
    }

    if (! name.isEmpty() &&
        ! email.isEmpty() && isEmailValid)
    {
        QtConcurrent::run([=] {
            emit disableElementsS(true);

            ui->btnSend->setText(tr("Sending.."));
            QStringList args;

            QJsonObject postData;
            postData["name"] = name;
            postData["email"] = email;
            postData["message"] = message;

            QJsonDocument json(postData);

            args << "-d" << json.toJson() << "-H" << mHeader << "-X" << "POST" << mFeedbackUrl;

            try {
                QString result = CommandUtil::exec("curl", args);
                QJsonObject response = QJsonDocument::fromJson(result.toUtf8()).object();

                if (response.value("success").toBool()) {
                    emit clearInputs();
                    emit setErrorMessageS(tr("<font color='#2ecc71'>Your Feedback has been successfully sended.</font>"));
                } else {
                    emit setErrorMessageS(tr("Something went wrong, try again !"));
                }

            } catch(QString &ex) {
                qCritical() << ex;
                emit setErrorMessageS(tr("Something went wrong, try again !"));
            }

            ui->btnSend->setText(tr("Save"));
            emit disableElementsS(false);
        });

    } else {
        emit setErrorMessageS(tr("Fields cannot be left blank !"));
    }
}

void Feedback::setErrorMessage(const QString &msg)
{
    ui->lblErrorMsg->setText(msg);
}

void Feedback::disableElements(const bool status)
{
    ui->txtName->setDisabled(status);
    ui->txtEmail->setDisabled(status);
    ui->txtMessage->setDisabled(status);
    ui->btnSend->setDisabled(status);
}

void Feedback::clearInputs()
{
    ui->txtName->clear();
    ui->txtEmail->clear();
    ui->txtMessage->clear();
    ui->txtName->setFocus();
}

void Feedback::on_btnClose_clicked()
{
    this->close();
}
