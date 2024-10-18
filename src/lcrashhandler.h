#ifndef LCRASHHANDLER_H
#define LCRASHHANDLER_H

#include "lukqtglobal.h"

#include <QString>

class LUKQT_DECLSPEC LCrashHandler
{
public:
    static void set(const QString &dirPath, const QString &applicationVersion = QString());
};

#endif // LCRASHHANDLER_H
