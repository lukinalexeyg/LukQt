#ifndef LLOG_H
#define LLOG_H

#include "lapputils.h"
#include "lmacros.h"

#include <QDebug>

class LFunctionLog
{
public:
    explicit LFunctionLog(const char *function) : m_function(function) { qDebug("+ %s", function); }

    ~LFunctionLog() { qDebug("- %s", m_function); }

private:
    const char* m_function;
};

#ifdef Q_OS_WIN
    #define FUNCTION_NAME __FUNCTION__
#else
    #define FUNCTION_NAME __PRETTY_FUNCTION__
#endif

#define FUNCTION_LOG LFunctionLog lFunctionLog(FUNCTION_NAME)

#define DEBUG_LOG_ qDebug().noquote() << FUNCTION_NAME
#define DEBUG_LOG DEBUG_LOG_ <<

#define INFO_LOG_ qInfo().noquote() << FUNCTION_NAME
#define INFO_LOG INFO_LOG_ <<

#define FUNCTION_NAME_AND_LINE QSL("%1:%2").arg(FUNCTION_NAME).arg(__LINE__)

#define WARNING_LOG_ qWarning().noquote() << FUNCTION_NAME_AND_LINE
#define WARNING_LOG WARNING_LOG_ <<

#define WARNING_LOG_E_ \
    LAppUtils::setLastError(); \
    qWarning().noquote() \
        << FUNCTION_NAME_AND_LINE \
        << QSL("(%1:%2)").arg(LAppUtils::lastError()).arg(LAppUtils::lastErrorString())

#define WARNING_LOG_E WARNING_LOG_E_ <<

#define CRITICAL_LOG_ qCritical().noquote() << FUNCTION_NAME_AND_LINE
#define CRITICAL_LOG CRITICAL_LOG_ <<

#define DUMP(x) " {" << STRINGIFY(x) << ":" << x << "} "

#endif // LLOG_H
