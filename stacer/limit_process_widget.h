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
    LimitProcessWidget(const QString& optionName, QWidget* parent = nullptr);
    bool limitSet;
    int counter;

public slots:
    void onSetCounterClicked();
    void onConfirmClicked();
};
#endif //STACER_LIMIT_PROCESS_WIDGET_H
