#include "network_info.h"
#include <QDebug>

NetworkInfo::NetworkInfo()
{
    for (const QNetworkInterface &net: QNetworkInterface::allInterfaces()) {

        if ((net.flags()  & QNetworkInterface::IsUp) &&
            (net.flags()  & QNetworkInterface::IsRunning) &&
            !(net.flags() & QNetworkInterface::IsLoopBack))
        {
            defaultNetworkInterface = net.name();
            break;
        }
    }

    rxPath = QString("/sys/class/net/%1/statistics/rx_bytes")
            .arg(defaultNetworkInterface);

    txPath = QString("/sys/class/net/%1/statistics/tx_bytes")
            .arg(defaultNetworkInterface);
}

QList<QNetworkInterface> NetworkInfo::getAllInterfaces()
{
    return QNetworkInterface::allInterfaces();
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
