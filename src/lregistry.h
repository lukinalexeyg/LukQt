#ifndef LREGISTRY_H
#define LREGISTRY_H

#include <QVariant>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

class LRegistry
{
public:
    static QVariant get(const QString &key, const QVariant &def = QVariant());

    static void set(const QString &key, const QVariant &value);

    static QStringList allKeys(const QString &path);

    static QStringList childKeys(const QString &path);

    static QStringList childGroups(const QString &path);

#ifdef Q_OS_WIN
    static QString readString(HKEY hKey, LPCWSTR valueName);

    static QString readString(HKEY hKey, LPCWSTR path, LPCWSTR valueName);

    static QByteArray readByteArray(HKEY hKey, LPCWSTR path, LPCWSTR valueName);

    static int readInt(HKEY hKey, LPCWSTR path, LPCWSTR valueName);

    static bool write(HKEY hKey, LPCWSTR path, LPCWSTR valueName, const QString &data, bool createSubKeys = true);

    static bool write(HKEY hKey, LPCWSTR path, LPCWSTR valueName, const QByteArray &data, bool createSubKeys = true);

    static bool write(HKEY hKey, LPCWSTR path, LPCWSTR valueName, int data, bool createSubKeys = true);

    static bool write(HKEY hKey, LPCWSTR path, LPCWSTR valueName, DWORD type, const LPBYTE, DWORD size, bool createSubKeys = true);

    static bool remove(HKEY hKey, LPCWSTR path);

    static bool remove(HKEY hKey, LPCWSTR path, LPCWSTR valueName);
#endif
};

#endif // LREGISTRY_H
