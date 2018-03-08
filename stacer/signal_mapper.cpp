#include "signal_mapper.h"

SignalMapper *SignalMapper::instance = nullptr;

SignalMapper* SignalMapper::ins()
{
    if (! instance) {
        instance = new SignalMapper;
    }
    return instance;
}
