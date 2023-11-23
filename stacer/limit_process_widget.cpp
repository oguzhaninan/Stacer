//
// Created by jordan on 11/18/23.
//
#include "limit_process_widget.h"
#include <QScreen>

LimitProcessWidget::LimitProcessWidget(const QString& optionName, QWidget* parent)
: QWidget(parent), counter(0)
{
    // The display box that shows when the user chooses which kind of limit they want
    QVBoxLayout* layout = new QVBoxLayout(this);

    /*
     * Used to determine whether the RAM Limit or CPU Limit command with be
     * sent to the shell.
     */
    currentOptionName = optionName;

    // Gives the display box a name that corresponds with the chosen limit type
    QLabel* nameLabel = new QLabel(optionName, this);
    layout->addWidget(nameLabel);

    // The text that corresponds to the value input in the display box
    QLabel* counterLabel = new QLabel(QString("Limit: %1").arg(counter), this);
    layout->addWidget(counterLabel);

    // The button the user presses to input a limit value
    QPushButton* setCounterButton = new QPushButton("Set Limit", this);
    // Connect the button to its behavior function below
    connect(setCounterButton, &QPushButton::clicked, this, &LimitProcessWidget::onSetCounterClicked);
    layout->addWidget(setCounterButton);

    // The confirm limit button
    QPushButton* confirmButton = new QPushButton("Confirm", this);
    // Connect the button to its behavior function below
    connect(confirmButton, &QPushButton::clicked, this, &LimitProcessWidget::onConfirmClicked);
    layout->addWidget(confirmButton);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();

    // move(screenGeometry.width() / 2 - width() / 2, screenGeometry.height() / 2 - height() / 2);
    move(screenGeometry.width() / 2, screenGeometry.height() / 2);
}


void LimitProcessWidget::onSetCounterClicked() {
    bool ok;
    // Gets the user input, maximum allowed for RAM is 2 GB
    if (currentOptionName == "RAM Limit: Bytes") {
        int value = QInputDialog::getInt(this, "Set Limit", "Enter limit value:", counter, 0, 2097152, 1, &ok);
        if (ok) {
            counter = value;
            // Update counter label
            static_cast<QLabel *>(layout()->itemAt(1)->widget())->setText(QString("Limit: %1").arg(counter));
        }
    }
    // Maximum allowed for CPU is 100%
    else if (currentOptionName == "CPU Limit: %") {
        int value = QInputDialog::getInt(this, "Set Limit", "Enter limit value:", counter, 0, 100, 1, &ok);
        if (ok) {
            counter = value;
            // Update counter label
            static_cast<QLabel *>(layout()->itemAt(1)->widget())->setText(QString("Limit: %1").arg(counter));
        }
    }
}

void LimitProcessWidget::onConfirmClicked() {
    // When the user clicks confirm, display a confirmation message
    QMessageBox messageBox;
    messageBox.setText(QString("Limit set to: %1").arg(counter));
    messageBox.setWindowTitle(QString("Success"));
    messageBox.exec();

    // Then activate the onLimitProcessConfirm function in the ProcessesPage class
    ProcessesPage::onLimitProcessConfirm(counter, currentOptionName);

    // Close the window
    this->close();
}


