#include "lregistry.h"

#include "llog.h"
#include "lmacros.h"

#include <QApplication>
#include <QSettings>



QVariant LRegistry::get(const QString &key, const QVariant &def)
{
    QSettings settings;
    return settings.value(key, def);
}



void LRegistry::set(const QString &key, const QVariant &value)
{
    QSettings settings;
    settings.setValue(key, value);
}



QStringList LRegistry::allKeys(const QString &path)
{
    QSettings settings(path, QSettings::NativeFormat);
    return settings.allKeys();
}



QStringList LRegistry::childKeys(const QString &path)
{
    QSettings settings(path, QSettings::NativeFormat);
    return settings.childKeys();
}



QStringList LRegistry::childGroups(const QString &path)
{
    QSettings settings(path, QSettings::NativeFormat);
    return settings.childGroups();
}



#ifdef Q_OS_WIN

QString LRegistry::readString(HKEY hKey, LPCWSTR valueName)
{
    QString result;
    DWORD dwSize = 0;

    LSTATUS status = RegQueryValueEx(hKey, valueName, NULL, NULL, NULL, &dwSize);

    if (status == ERROR_SUCCESS) {
        DWORD dwType = REG_SZ;
        LPBYTE pBuffer = new BYTE[dwSize];

        status = RegQueryValueEx(hKey, valueName, NULL, &dwType, pBuffer, &dwSize);

        if (status == ERROR_SUCCESS)
            result = QString::fromWCharArray(R_CAST_LPWSTR(pBuffer));

        delete [] pBuffer;
    }

    if (status != ERROR_SUCCESS)
        WARNING_LOG status;

    return result;
}



QString LRegistry::readString(HKEY hKey, LPCWSTR path, LPCWSTR valueName)
{
    HKEY phkResult;
    QString result;

    LSTATUS status = RegOpenKeyEx(hKey, path, 0, KEY_READ | KEY_WOW64_64KEY, &phkResult);

    if (status == ERROR_SUCCESS) {
        DWORD dwType = REG_SZ;
        DWORD dwSize = 0;

        status = RegQueryValueEx(phkResult, valueName, NULL, &dwType, NULL, &dwSize);

        if (status == ERROR_SUCCESS) {
            LPBYTE pBuffer = new BYTE[dwSize];

            status = RegQueryValueEx(phkResult, valueName, NULL, &dwType, pBuffer, &dwSize);

            if (status == ERROR_SUCCESS)
                result = QString::fromWCharArray(R_CAST_LPWSTR(pBuffer));

            delete [] pBuffer;
        }

        RegCloseKey(phkResult);
    }

    if (status != ERROR_SUCCESS)
        WARNING_LOG status;

    return result;
}



QByteArray LRegistry::readByteArray(HKEY hKey, LPCWSTR path, LPCWSTR valueName)
{
    HKEY phkResult;
    QByteArray result;

    LSTATUS status = RegOpenKeyEx(hKey, path, 0, KEY_READ | KEY_WOW64_64KEY, &phkResult);

    if (status == ERROR_SUCCESS) {
        DWORD dwType = REG_BINARY;
        DWORD dwSize = 0;

        status = RegQueryValueEx(phkResult, valueName, NULL, &dwType, NULL, &dwSize);

        if (status == ERROR_SUCCESS) {
            LPBYTE pBuffer = new BYTE[dwSize];

            status = RegQueryValueEx(phkResult, valueName, NULL, &dwType, pBuffer, &dwSize);

            if (status == ERROR_SUCCESS)
                result = QByteArray(R_CAST_LPSTR(pBuffer), dwSize);

            delete [] pBuffer;
        }

        RegCloseKey(phkResult);
    }

    if (status != ERROR_SUCCESS)
        WARNING_LOG status;

    return result;
}



int LRegistry::readInt(HKEY hKey, LPCWSTR path, LPCWSTR valueName)
{
    HKEY phkResult;
    int result = 0;

    LSTATUS status = RegOpenKeyEx(hKey, path, 0, KEY_READ | KEY_WOW64_64KEY, &phkResult);

    if (status == ERROR_SUCCESS) {
        DWORD dwType = REG_DWORD;
        DWORD data = 0;
        DWORD dwSize = 0;

        status = RegQueryValueEx(phkResult, valueName, NULL, &dwType, (LPBYTE)&data, &dwSize);

        if (status == ERROR_SUCCESS)
            result = S_CAST_INT(data);

        RegCloseKey(phkResult);
    }

    if (status != ERROR_SUCCESS)
        WARNING_LOG status;

    return result;
}



bool LRegistry::write(HKEY hKey, LPCWSTR path, LPCWSTR valueName, const QString &data, const bool createSubKeys)
{
    return write(hKey, path, valueName, REG_SZ, (const LPBYTE)data.utf16(), data.size()*2, createSubKeys);
}



bool LRegistry::write(HKEY hKey, LPCWSTR path, LPCWSTR valueName, const QByteArray &data, const bool createSubKeys)
{
    return write(hKey, path, valueName, REG_BINARY, (const LPBYTE)data.data(), data.size(), createSubKeys);
}



bool LRegistry::write(HKEY hKey, LPCWSTR path, LPCWSTR valueName, const int data, const bool createSubKeys)
{
    return write(hKey, path, valueName, REG_DWORD, (const LPBYTE)&data, sizeof(DWORD), createSubKeys);
}



bool LRegistry::write(HKEY hKey,
                     LPCWSTR path,
                     LPCWSTR valueName,
                     const DWORD type,
                     const LPBYTE data,
                     const DWORD size,
                     const bool createSubKeys)
{
    HKEY phkResult;

    if (createSubKeys)
        RegCreateKey(hKey, path, &phkResult);

    LSTATUS status = RegOpenKeyEx(hKey, path, 0, KEY_WRITE | KEY_WOW64_64KEY, &phkResult);

    if (status == ERROR_SUCCESS) {
        status = RegSetValueEx(phkResult,
                               valueName,
                               NULL,
                               type,
                               data,
                               size);

        RegCloseKey(phkResult);
    }

    if (status == ERROR_SUCCESS)
        return true;

    WARNING_LOG status;

    return false;
}



bool LRegistry::remove(HKEY hKey, LPCWSTR path)
{
    const LSTATUS status = RegDeleteKey(hKey, path);

    if (status == ERROR_SUCCESS)
        return true;

    WARNING_LOG status;

    return false;
}



bool LRegistry::remove(HKEY hKey, LPCWSTR path, LPCWSTR valueName)
{
    HKEY phkResult;

    LSTATUS status = RegOpenKeyEx(hKey, path, 0, KEY_WRITE | KEY_WOW64_64KEY, &phkResult);

    if (status == ERROR_SUCCESS) {
        status = RegDeleteValue(phkResult, valueName);

        RegCloseKey(phkResult);
    }

    if (status == ERROR_SUCCESS)
        return true;

    WARNING_LOG status;

    return false;
}

#endif
