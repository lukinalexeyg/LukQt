#ifndef LAPPUTILS_H
#define LAPPUTILS_H

#include "lukqtglobal.h"

#include <QDateTime>
#include <QSharedMemory>

class LUKQT_DECLSPEC LAppUtils
{
public:
    static QStringList argumentsList(int argc, char **argv);

    static QDateTime buildDateTime();

    static void exit(int returnCode = 0, int delay = 0);

    static bool isRunning(const QString &systemSemaphoreKey, QSharedMemory &sharedMemory, bool create);

    static bool areRunning(const QStringList &processNames);

    static void errorBeep();
};

#endif // LAPPUTILS_H
