#include "lapputils.h"

#include "lmacros.h"

#include <QCoreApplication>
#include <QLocale>
#include <QSystemSemaphore>
#include <QTimer>
#include <QVector>

#ifdef Q_OS_WIN
    #include <comdef.h>
    #include <TlHelp32.h>
    #include <Windows.h>
#else
    #include <dirent.h>
    #include <fcntl.h>
    #include <fstream>
#endif

ulong LAppUtils::s_lastError = 0;



QDate LAppUtils::buildDate()
{
    return QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QSL("MMM d yyyy"));
}



QTime LAppUtils::buildTime()
{
    return QLocale(QLocale::C).toTime(QString(__TIME__).simplified(), QSL("hh:mm:ss"));
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

    // Open the /proc directory
    DIR *procDir = opendir("/proc");

    if (procDir != NULL) {
        // Enumerate all entries in directory until process found
        struct dirent *dirEntity;

        while ((dirEntity = readdir(procDir))) {
            // Skip non-numeric entries
            const int pid = atoi(dirEntity->d_name);

            if (pid > 0) {
                // Read contents of virtual /proc/{pid}/cmdline file
                std::string cmdPath = std::string("/proc/") + dirEntity->d_name + "/cmdline";
                std::ifstream cmdFile(cmdPath.c_str());
                std::string cmdLine;
                getline(cmdFile, cmdLine);

                if (!cmdLine.empty()) {
                    // Keep first cmdline item which contains the program path
                    size_t pos = cmdLine.find('\0');
                    if (pos != std::string::npos)
                        cmdLine = cmdLine.substr(0, pos);

                    // Keep program name only, removing the path
                    pos = cmdLine.rfind('/');
                    if (pos != std::string::npos)
                        cmdLine = cmdLine.substr(pos + 1);

                    // Compare against requested process name
                    if (processNames.contains(QString::fromStdString(cmdLine)))
                        return true;
                }
            }
        }
    }

#endif

    return false;
}



void LAppUtils::setLastError()
{
#ifdef Q_OS_WIN
    s_lastError = GetLastError();
#else
    s_lastError = errno;
#endif
}



QString LAppUtils::lastErrorString()
{
    if (s_lastError == 0)
        return QString();

#ifdef Q_OS_WIN
    const _com_error comError(s_lastError);
    return QString::fromWCharArray(comError.ErrorMessage());
#else
    return QString::fromStdString(std::system_category().message(s_lastError));
#endif
}



void LAppUtils::errorBeep()
{
#ifdef Q_OS_WIN
    MessageBeep(MB_ICONERROR);
#else
    open("/dev/console", O_WRONLY);
#endif
}
