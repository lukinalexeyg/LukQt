#include "lcrashhandler.h"

#include "llog.h"
#include "lmacros.h"
#include "lpath.h"
#include "lstringutils.h"

#include <QDateTime>
#include <QDir>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <DbgHelp.h>
#include <Psapi.h>
#include <strsafe.h>
#include <TlHelp32.h>
#include <comdef.h>
#else
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#endif

static QString s_dirPath;
static QString s_applicationVersion;



#ifdef Q_OS_WIN

int generateDump(EXCEPTION_POINTERS *exceptionPointers)
{
    if (!QDir().mkpath(s_dirPath)) {
        WARNING_LOG_E "error create directory:" << s_dirPath;
        return EXCEPTION_EXECUTE_HANDLER;
    }

    QString fileName = LPath::combine(s_dirPath, QDateTime::currentDateTime().toString(QSL("yyMMdd-hhmmss")));

    if (!s_applicationVersion.isEmpty())
        fileName.append(QSL("-%1").arg(s_applicationVersion));

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

    CloseHandle(hDumpFile);

    return EXCEPTION_EXECUTE_HANDLER;
}



LONG WINAPI exceptionFilter(EXCEPTION_POINTERS *exceptionPointers)
{
    generateDump(exceptionPointers);
    return EXCEPTION_CONTINUE_SEARCH;
}



#else // Q_OS_LINUX

void signalHandler(const int signum)
{
    if (!QDir().mkpath(s_dirPath)) {
        WARNING_LOG_E "error create directory:" << s_dirPath;
        return;
    }

    QString fileName = LPath::combine(s_dirPath, QDateTime::currentDateTime().toString(QSL("yyMMdd-hhmmss")));

    if (!s_applicationVersion.isEmpty())
        fileName.append(QSL("-%1").arg(s_applicationVersion));

    fileName.append(QSL(".log"));

    const int fd = ::open(fileName.toLocal8Bit().constData(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
        return;

    void *array[50];
    const size_t size = backtrace(array, 50);
    backtrace_symbols_fd(array, size, fd);
    ::close(fd);

    ::signal(signum, SIG_DFL);
    ::raise(signum);
}

#endif



void LCrashHandler::set(const QString &dirPath, const QString &applicationVersion)
{
    s_dirPath = dirPath;
    s_applicationVersion = applicationVersion;

#ifdef Q_OS_WIN
    SetErrorMode(SEM_NOGPFAULTERRORBOX);
    SetUnhandledExceptionFilter(R_CAST(LPTOP_LEVEL_EXCEPTION_FILTER, exceptionFilter));
#else
    ::signal(SIGSEGV, signalHandler);
    ::signal(SIGABRT, signalHandler);
    ::signal(SIGFPE, signalHandler);
    ::signal(SIGILL, signalHandler);
#endif
}
