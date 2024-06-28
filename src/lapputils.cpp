#include "lapputils.h"

#include "lmacros.h"

#include <QCoreApplication>
#include <QLocale>
#include <QSystemSemaphore>
#include <QTimer>
#include <QVector>

#if defined(Q_OS_WIN)
    #include <Windows.h>
    #include <TlHelp32.h>
#elif defined(Q_OS_UNIX)
    #include <fcntl.h>
#endif



#ifdef Q_OS_WIN
// taken from qcoreapplication_p.h
template<typename Char>
static QVector<Char*> qWinCmdLine(Char *cmdParam, int length, int &argc)
{
    QVector<Char*> argv(8);
    Char *p = cmdParam;
    Char *p_end = p + length;

    argc = 0;

    while (*p && p < p_end) {                                // parse cmd line arguments
        while (QChar((short)(*p)).isSpace())                 // skip white space
            p++;
        if (*p && p < p_end) {                               // arg starts
            int quote;
            Char *start, *r;
            if (*p == Char('\"') || *p == Char('\'')) {      // " or ' quote
                quote = *p;
                start = ++p;
            } else {
                quote = 0;
                start = p;
            }
            r = start;
            while (*p && p < p_end) {
                if (quote) {
                    if (*p == quote) {
                        p++;
                        if (QChar((short)(*p)).isSpace())
                            break;
                        quote = 0;
                    }
                }
                if (*p == '\\') {                           // escape char?
                    p++;
                    if (*p == Char('\"') || *p == Char('\''))
                        ;                                   // yes
                    else
                        p--;                                // treat \ literally
                } else {
                    if (!quote && (*p == Char('\"') || *p == Char('\''))) { // " or ' quote
                        quote = *p++;
                        continue;
                    } else if (QChar((short)(*p)).isSpace() && !quote)
                        break;
                }
                if (*p)
                    *r++ = *p++;
            }
            if (*p && p < p_end)
                p++;
            *r = Char('\0');

            if (argc >= (int)argv.size()-1)                 // expand array
                argv.resize(argv.size()*2);
            argv[argc++] = start;
        }
    }
    argv[argc] = nullptr;

    return argv;
}



QStringList LAppUtils::argumentsList(int argc, char **argv)
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    QStringList arguments;
    QString cmdLine = QString::fromWCharArray(GetCommandLine());

    QVector<wchar_t*> args = qWinCmdLine<wchar_t>((wchar_t *)cmdLine.utf16(), cmdLine.length(), argc);

    for (int a = 0; a < argc; ++a)
        arguments << QString::fromWCharArray(args[a]);

    return arguments;
}
#else
QStringList LAppUtils::argumentsList(int argc, char **argv)
{
    QStringList arguments;

    foreach_index_inc (a, argc)
        arguments << QString::fromLocal8Bit(argv[a]);

    return arguments;
}
#endif



QDateTime LAppUtils::buildDateTime()
{
    const QDate date = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QSL("MMM d yyyy"));
    const QTime time = QLocale(QLocale::C).toTime(QString(__TIME__).simplified(), QSL("hh:mm:ss"));

    return QDateTime(date, time);
}



void LAppUtils::exit(const int returnCode, const int delay)
{
    QTimer::singleShot(delay, qApp, [returnCode] { qApp->exit(returnCode); });
}



bool LAppUtils::isRunning(const QString &systemSemaphoreKey, QSharedMemory &sharedMemory, const bool create)
{
    QSystemSemaphore systemSemaphore(systemSemaphoreKey, QSystemSemaphore::Create);
    bool isRunning = false;

    systemSemaphore.acquire();

    if (sharedMemory.attach())
        isRunning = true;
    else if (create)
        sharedMemory.create(QSystemSemaphore::Create);

    systemSemaphore.release();

    return isRunning;
}



bool LAppUtils::areRunning(const QStringList &processNames)
{
#ifdef Q_OS_WIN
    const HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (hSnapshot == INVALID_HANDLE_VALUE)
        return false;

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(processEntry);
    BOOL bResult = Process32First(hSnapshot, &processEntry);

    while (bResult) {
        if (processNames.contains(QString::fromWCharArray(processEntry.szExeFile))) {
            /*
            CONST HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, NULL, processEntry.th32ProcessID);
            if (hProcess != nullptr) {
                DWORD dwExitCode = 0;
                if (GetExitCodeProcess(hProcess, &dwExitCode))
                    INFO_LOG processEntry.th32ProcessID << dump(processName) << dump(dwExitCode);
                CloseHandle(hProcess);
            }
            */
            CloseHandle(hSnapshot);
            return true;
        }
        bResult = Process32Next(hSnapshot, &processEntry);
    }

    CloseHandle(hSnapshot);
#else
    Q_UNUSED(processNames)
#endif

    return false;
}



void LAppUtils::errorBeep()
{
#ifdef Q_OS_WIN
    MessageBeep(MB_ICONERROR);
#else
    open("/dev/console", O_WRONLY);
#endif
}
