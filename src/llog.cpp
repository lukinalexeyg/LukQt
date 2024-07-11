#include "llog.h"

#ifdef Q_OS_WIN
    #include <Windows.h>
    #include <comdef.h>
#endif



QString lastErrorString()
{
#ifdef Q_OS_WIN
    const DWORD dwCode = GetLastError();

    if (dwCode == 0)
        return QString::number(dwCode);

    const _com_error comError(dwCode);

    return QStringLiteral("%1:%2")
            .arg(dwCode)
            .arg(QString::fromWCharArray(comError.ErrorMessage()));
#else
    return QString::number(errno);
#endif
}
