#ifndef LLOG_H
#define LLOG_H

#include "lukqtglobal.h"

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

#define DEBUG_LOG qDebug().noquote() << FUNCTION_NAME <<
#define DEBUG_LOG_ qDebug().noquote() << FUNCTION_NAME

#define INFO_LOG qInfo().noquote() << FUNCTION_NAME <<
#define INFO_LOG_ qInfo().noquote() << FUNCTION_NAME

#define FUNCTION_LINE QStringLiteral("%1:%2:").arg(FUNCTION_NAME).arg(__LINE__)

#ifdef Q_OS_WIN
    LUKQT_DECLSPEC QString lastErrorString();
    #define FUNCTION_LINE_E QStringLiteral("%1 [%2]").arg(FUNCTION_LINE).arg(lastErrorString())
#else
    #define FUNCTION_LINE_E FUNCTION_LINE
#endif

#define WARNING_LOG qWarning().noquote() << FUNCTION_LINE <<
#define WARNING_LOG_ qWarning().noquote() << FUNCTION_LINE
#define WARNING_LOG_E qWarning().noquote() << FUNCTION_LINE_E <<
#define WARNING_LOG_E_ qWarning().noquote() << FUNCTION_LINE_E

#define CRITICAL_LOG qCritical().noquote() << FUNCTION_LINE <<
#define CRITICAL_LOG_E qCritical().noquote() << FUNCTION_LINE_E <<

#define FATAL_LOG qFatal().noquote() << FUNCTION_LINE <<
#define FATAL_LOG_E qFatal().noquote() << FUNCTION_LINE_E <<

#define DUMP(x) " {" << #x << ":" << x << "} "

#endif // LLOG_H
