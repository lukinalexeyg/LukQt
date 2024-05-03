#include "lnetworkutils.h"

#include "lchars.h"
#include "lmacros.h"
#include "lstringutils.h"

#include <QCoreApplication>
#include <QNetworkInterface>
#include <QUrl>



QHostAddress LNetworkUtils::localIPAddress(const QString &subnet)
{
    const QPair<QHostAddress, int> subnetPair = QHostAddress::parseSubnet(subnet);
    QMap<QString, QHostAddress> map;

    foreach_element_ref (hostAddress, QNetworkInterface::allAddresses())
        if (hostAddress.protocol() == QAbstractSocket::IPv4Protocol
            && hostAddress != QHostAddress(QHostAddress::LocalHost)
            && (subnet.isEmpty() || hostAddress.isInSubnet(subnetPair)))
        map.insert(hostAddress.toString(), hostAddress);

    return map.isEmpty() ? QHostAddress() : map.first();
}



QString LNetworkUtils::urlOrigin(const QUrl &url)
{
    return QSL("%1://%2").arg(url.scheme(), url.authority());
}



QString LNetworkUtils::urlResource(const QUrl &url)
{
    QString _resource = url.path();

    if (url.hasQuery())
        _resource += QSL("?%1").arg(url.query());

    return _resource;
}
