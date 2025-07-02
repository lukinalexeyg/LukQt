#ifndef LAPPUTILS_H
#define LAPPUTILS_H

#include "lukqtglobal.h"

#include <QDateTime>
#include <QSharedMemory>

class LUKQT_DECLSPEC LAppUtils
{
public:
    static QDate buildDate();

    static QTime buildTime();

    static void exit(int returnCode = 0, int delay = 0);

    static bool isRunning(const QString &systemSemaphoreKey, QSharedMemory &sharedMemory, bool createIfMissing);

    static bool areRunning(const QStringList &processNames);

    static void setLastError();

    static ulong lastError() { return s_lastError; }

    static QString lastErrorString();

    static void errorBeep();

private:
    static ulong s_lastError;
};

#endif // LAPPUTILS_H
