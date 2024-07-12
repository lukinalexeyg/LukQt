#ifndef LREGISTRY_H
#define LREGISTRY_H

#include "lukqtglobal.h"

#include <QVariant>

#ifdef Q_OS_WIN
    #include <Windows.h>
#endif

class LUKQT_DECLSPEC LRegistry
{
public:
    static QVariant get(const QString &key, const QVariant &def = QVariant());

    static void set(const QString &key, const QVariant &value);

    static QStringList allKeys(const QString &path);

    static QStringList childKeys(const QString &path);

    static QStringList childGroups(const QString &path);

#ifdef Q_OS_WIN
    static QString readString(HKEY hKey, LPCWSTR valueName, LSTATUS *status = nullptr);

    static QString readString(HKEY hKey, LPCWSTR path, LPCWSTR valueName, LSTATUS *status = nullptr);

    static QByteArray readByteArray(HKEY hKey, LPCWSTR path, LPCWSTR valueName, LSTATUS *status = nullptr);

    static int readInt(HKEY hKey, LPCWSTR path, LPCWSTR valueName, LSTATUS *status = nullptr);

    static bool write(HKEY hKey,
                      LPCWSTR path,
                      LPCWSTR valueName,
                      const QString &data,
                      bool createSubKeys = true,
                      LSTATUS *status = nullptr);

    static bool write(HKEY hKey,
                      LPCWSTR path,
                      LPCWSTR valueName,
                      const QByteArray &data,
                      bool createSubKeys = true,
                      LSTATUS *status = nullptr);

    static bool write(HKEY hKey,
                      LPCWSTR path,
                      LPCWSTR valueName,
                      int data,
                      bool createSubKeys = true,
                      LSTATUS *status = nullptr);

    static bool write(HKEY hKey,
                      LPCWSTR path,
                      LPCWSTR valueName,
                      DWORD type,
                      const LPBYTE,
                      DWORD size,
                      bool createSubKeys = true,
                      LSTATUS *status = nullptr);

    static bool remove(HKEY hKey, LPCWSTR path, LSTATUS *status = nullptr);

    static bool remove(HKEY hKey, LPCWSTR path, LPCWSTR valueName, LSTATUS *status = nullptr);
#endif
};

#endif // LREGISTRY_H
