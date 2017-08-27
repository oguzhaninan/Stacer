#ifndef NETWORK_INFO_H
#define NETWORK_INFO_H

#include <QObject>
#include <QDebug>

#include "Utils/file_util.h"
#include "Utils/command_util.h"

#define PROC_NET_ROUTE "/proc/net/route"

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT NetworkInfo : public QObject
{
    Q_OBJECT

public:
    NetworkInfo();

    QString getDefaultNetworkInterface() const;

    quint64 getRXbytes();
    quint64 getTXbytes();

private:
    QString defaultNetworkInterface;

    QString rxPath;
    QString txPath;
};

#endif // NETWORK_INFO_H
