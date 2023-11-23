//
// Created by jordan on 11/18/23.
//

#ifndef STACER_LIMIT_PROCESS_WIDGET_H
#define STACER_LIMIT_PROCESS_WIDGET_H

#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include "processes_page.h"

class LimitProcessWidget : public QWidget {
    Q_OBJECT

public:
    LimitProcessWidget(const QString& optionName, QWidget* parent = nullptr); // Constructor
    int counter; // User input value for limit quantity
    QString currentOptionName; // Used to determine which command to send to the shell


public slots:
    void onSetCounterClicked(); // Button behavior for setting limit quantity
    void onConfirmClicked();    // Button behavior for confirming limit quantity

};
#endif //STACER_LIMIT_PROCESS_WIDGET_H
