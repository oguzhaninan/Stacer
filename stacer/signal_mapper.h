#ifndef SIGNAL_MAPPER_H
#define SIGNAL_MAPPER_H

#include <QObject>

class SignalMapper : public QObject
{
    Q_OBJECT

public:
    static SignalMapper *ins() {
        static SignalMapper *instance = new SignalMapper;
        return instance;
    }

signals:
    void changedAppTheme();
};

#endif // SIGNAL_MAPPER_H
