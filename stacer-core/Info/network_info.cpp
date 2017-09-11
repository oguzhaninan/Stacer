#include "network_info.h"

NetworkInfo::NetworkInfo()
{
    QStringList lines = FileUtil::readListFromFile(PROC_NET_ROUTE);

    if(lines.count() > 2)
        defaultNetworkInterface = lines
                .at(2)
                .split(QRegExp("\\s+"))
                .first();
    else
        defaultNetworkInterface = "";

    rxPath = QString("/sys/class/net/%1/statistics/rx_bytes")
            .arg(defaultNetworkInterface);

    txPath = QString("/sys/class/net/%1/statistics/tx_bytes")
            .arg(defaultNetworkInterface);
}

QString NetworkInfo::getDefaultNetworkInterface() const
{
    return defaultNetworkInterface;
}

quint64 NetworkInfo::getRXbytes() const
{
    quint64 rx = FileUtil::readStringFromFile(rxPath)
            .trimmed()
            .toLong();

    return rx;
}

quint64 NetworkInfo::getTXbytes() const
{
    quint64 tx = FileUtil::readStringFromFile(txPath)
            .trimmed()
            .toLong();

    return tx;
}
