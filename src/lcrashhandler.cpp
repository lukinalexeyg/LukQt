#include "lcrashhandler.h"

#ifdef Q_OS_WIN

#include "llog.h"
#include "lmacros.h"
#include "lpath.h"
#include "lstringutils.h"

#include <QDateTime>
#include <QDir>

#include <comdef.h>
#include <DbgHelp.h>
#include <Psapi.h>
#include <strsafe.h>
#include <TlHelp32.h>
#include <Windows.h>

static QString s_dirPath;
static QString s_version;



int generateDump(EXCEPTION_POINTERS *exceptionPointers)
{
    if (!QDir().mkpath(s_dirPath)) {
        WARNING_LOG_E "error create directory:" << s_dirPath;
        return EXCEPTION_EXECUTE_HANDLER;
    }

    QString fileName = LPath::combine(s_dirPath, QDateTime::currentDateTime().toString(QSL("yyMMdd-hhmmss")));

    if (!s_version.isEmpty())
        fileName.append(QSL("-%1").arg(s_version));

    fileName.append(QSL(".dmp"));

    HANDLE hDumpFile = CreateFile(LStringUtils::toConstWCharArray(fileName),
                                  GENERIC_READ | GENERIC_WRITE,
                                  FILE_SHARE_WRITE | FILE_SHARE_READ,
                                  nullptr,
                                  CREATE_ALWAYS,
                                  0,
                                  nullptr);

    MINIDUMP_EXCEPTION_INFORMATION exceptionInformation;
    exceptionInformation.ThreadId = GetCurrentThreadId();
    exceptionInformation.ExceptionPointers = exceptionPointers;
    exceptionInformation.ClientPointers = TRUE;

    MiniDumpWriteDump(GetCurrentProcess(),
                      GetCurrentProcessId(),
                      hDumpFile,
                      MiniDumpWithDataSegs,
                      &exceptionInformation,
                      nullptr,
                      nullptr);

    return EXCEPTION_EXECUTE_HANDLER;
}



LONG WINAPI exceptionFilter(EXCEPTION_POINTERS *exceptionPointers)
{
    generateDump(exceptionPointers);
    return EXCEPTION_CONTINUE_SEARCH;
}

#endif



void LCrashHandler::set(const QString &dirPath, const QString &version)
{
#ifdef Q_OS_WIN
    s_dirPath = dirPath;
    s_version = version;

    SetErrorMode(SEM_NOGPFAULTERRORBOX);
    SetUnhandledExceptionFilter(R_CAST(LPTOP_LEVEL_EXCEPTION_FILTER, exceptionFilter));

#else
    Q_UNUSED(dirPath);
    Q_UNUSED(version);
#endif
}
