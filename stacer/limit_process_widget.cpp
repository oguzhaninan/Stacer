//
// Created by jordan on 11/18/23.
//
#include "limit_process_widget.h"
#include <QScreen>

LimitProcessWidget::LimitProcessWidget(const QString& optionName, QWidget* parent)
: QWidget(parent), counter(0)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    currentOptionName = optionName;

    QLabel* nameLabel = new QLabel(optionName, this);
    layout->addWidget(nameLabel);

    QLabel* counterLabel = new QLabel(QString("Limit: %1").arg(counter), this);
    layout->addWidget(counterLabel);

    QPushButton* setCounterButton = new QPushButton("Set Counter", this);
    connect(setCounterButton, &QPushButton::clicked, this, &LimitProcessWidget::onSetCounterClicked);
    layout->addWidget(setCounterButton);

    QPushButton* confirmButton = new QPushButton("Confirm", this);
    connect(confirmButton, &QPushButton::clicked, this, &LimitProcessWidget::onConfirmClicked);
    layout->addWidget(confirmButton);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();

    move(screenGeometry.width() / 2 - width() / 2, screenGeometry.height() / 2 - height() / 2);
}


void LimitProcessWidget::onSetCounterClicked() {
    bool ok;
    int value = QInputDialog::getInt(this, "Set Counter", "Enter counter value:", counter, 0, 2097152, 1, &ok);
    if (ok) {
        counter = value;
        // Update counter label
        static_cast<QLabel*>(layout()->itemAt(1)->widget())->setText(QString("Counter: %1").arg(counter));
    }
}

void LimitProcessWidget::onConfirmClicked() {
    QMessageBox messageBox;
    messageBox.setText(QString("Counter set to: %1").arg(counter));
    messageBox.setWindowTitle(QString("Success"));
    messageBox.exec();

    ProcessesPage::onLimitProcessConfirm(counter, currentOptionName);
    limitSet = true;
    this->close();

}


