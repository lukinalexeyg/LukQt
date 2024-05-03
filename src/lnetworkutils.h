#ifndef LNETWORKUTILS_H
#define LNETWORKUTILS_H

#include "lukqtglobal.h"

#include <QHostAddress>
#include <QHostInfo>

class LUKQT_DECLSPEC LNetworkUtils
{
public:
    static QString localHostName() { return QHostInfo::localHostName(); }

    static QHostAddress localIPAddress(const QString &subnet = QString("192.168.0.0/16"));

    static QString urlOrigin(const QUrl &url);

    static QString urlResource(const QUrl &url);
};

#endif // LNETWORKUTILS_H
