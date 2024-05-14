#include "lregistry.h"

#include "lmacros.h"

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

QString LRegistry::readString(HKEY hKey, LPCWSTR valueName, LSTATUS *status)
{
    QString result;
    DWORD dwSize = 0;

    LSTATUS _status = RegQueryValueEx(hKey, valueName, NULL, NULL, NULL, &dwSize);

    if (_status == ERROR_SUCCESS) {
        DWORD dwType = REG_SZ;
        LPBYTE pBuffer = new BYTE[dwSize];

        _status = RegQueryValueEx(hKey, valueName, NULL, &dwType, pBuffer, &dwSize);

        if (_status == ERROR_SUCCESS)
            result = QString::fromWCharArray(R_CAST_LPWSTR(pBuffer));

        delete [] pBuffer;
    }

    if (status != nullptr)
        *status = _status;

    return result;
}



QString LRegistry::readString(HKEY hKey, LPCWSTR path, LPCWSTR valueName, LSTATUS *status)
{
    HKEY phkResult;
    QString result;

    LSTATUS _status = RegOpenKeyEx(hKey, path, 0, KEY_READ | KEY_WOW64_64KEY, &phkResult);

    if (_status == ERROR_SUCCESS) {
        DWORD dwType = REG_SZ;
        DWORD dwSize = 0;

        _status = RegQueryValueEx(phkResult, valueName, NULL, &dwType, NULL, &dwSize);

        if (_status == ERROR_SUCCESS) {
            LPBYTE pBuffer = new BYTE[dwSize];

            _status = RegQueryValueEx(phkResult, valueName, NULL, &dwType, pBuffer, &dwSize);

            if (_status == ERROR_SUCCESS)
                result = QString::fromWCharArray(R_CAST_LPWSTR(pBuffer));

            delete [] pBuffer;
        }

        RegCloseKey(phkResult);
    }

    if (status != nullptr)
        *status = _status;

    return result;
}



QByteArray LRegistry::readByteArray(HKEY hKey, LPCWSTR path, LPCWSTR valueName, LSTATUS *status)
{
    HKEY phkResult;
    QByteArray result;

    LSTATUS _status = RegOpenKeyEx(hKey, path, 0, KEY_READ | KEY_WOW64_64KEY, &phkResult);

    if (_status == ERROR_SUCCESS) {
        DWORD dwType = REG_BINARY;
        DWORD dwSize = 0;

        _status = RegQueryValueEx(phkResult, valueName, NULL, &dwType, NULL, &dwSize);

        if (_status == ERROR_SUCCESS) {
            LPBYTE pBuffer = new BYTE[dwSize];

            _status = RegQueryValueEx(phkResult, valueName, NULL, &dwType, pBuffer, &dwSize);

            if (_status == ERROR_SUCCESS)
                result = QByteArray(R_CAST_LPSTR(pBuffer), dwSize);

            delete [] pBuffer;
        }

        RegCloseKey(phkResult);
    }

    if (status != nullptr)
        *status = _status;

    return result;
}



int LRegistry::readInt(HKEY hKey, LPCWSTR path, LPCWSTR valueName, LSTATUS *status)
{
    HKEY phkResult;
    int result = 0;

    LSTATUS _status = RegOpenKeyEx(hKey, path, 0, KEY_READ | KEY_WOW64_64KEY, &phkResult);

    if (_status == ERROR_SUCCESS) {
        DWORD dwType = REG_DWORD;
        DWORD data = 0;
        DWORD dwSize = 0;

        _status = RegQueryValueEx(phkResult, valueName, NULL, &dwType, (LPBYTE)&data, &dwSize);

        if (_status == ERROR_SUCCESS)
            result = S_CAST_INT(data);

        RegCloseKey(phkResult);
    }

    if (status != nullptr)
        *status = _status;

    return result;
}



bool LRegistry::write(HKEY hKey,
                      LPCWSTR path,
                      LPCWSTR valueName,
                      const QString &data,
                      const bool createSubKeys,
                      LSTATUS *status)
{
    return write(hKey, path, valueName, REG_SZ, (const LPBYTE)data.utf16(), data.size()*2, createSubKeys, status);
}



bool LRegistry::write(HKEY hKey,
                      LPCWSTR path,
                      LPCWSTR valueName,
                      const QByteArray &data,
                      const bool createSubKeys,
                      LSTATUS *status)
{
    return write(hKey, path, valueName, REG_BINARY, (const LPBYTE)data.data(), data.size(), createSubKeys, status);
}



bool LRegistry::write(HKEY hKey,
                      LPCWSTR path,
                      LPCWSTR valueName,
                      const int data,
                      const bool createSubKeys,
                      LSTATUS *status)
{
    return write(hKey, path, valueName, REG_DWORD, (const LPBYTE)&data, sizeof(DWORD), createSubKeys, status);
}



bool LRegistry::write(HKEY hKey,
                      LPCWSTR path,
                      LPCWSTR valueName,
                      const DWORD type,
                      const LPBYTE data,
                      const DWORD size,
                      const bool createSubKeys,
                      LSTATUS *status)
{
    HKEY phkResult;

    if (createSubKeys)
        RegCreateKey(hKey, path, &phkResult);

    LSTATUS _status = RegOpenKeyEx(hKey, path, 0, KEY_WRITE | KEY_WOW64_64KEY, &phkResult);

    if (_status == ERROR_SUCCESS) {
        _status = RegSetValueEx(phkResult,
                                valueName,
                                NULL,
                                type,
                                data,
                                size);

        RegCloseKey(phkResult);
    }

    if (status != nullptr)
        *status = _status;

    return _status == ERROR_SUCCESS;
}



bool LRegistry::remove(HKEY hKey, LPCWSTR path, LSTATUS *status)
{
    const LSTATUS _status = RegDeleteKey(hKey, path);

    if (status != nullptr)
        *status = _status;

    return _status == ERROR_SUCCESS;
}



bool LRegistry::remove(HKEY hKey, LPCWSTR path, LPCWSTR valueName, LSTATUS *status)
{
    HKEY phkResult;

    LSTATUS _status = RegOpenKeyEx(hKey, path, 0, KEY_WRITE | KEY_WOW64_64KEY, &phkResult);

    if (_status == ERROR_SUCCESS) {
        _status = RegDeleteValue(phkResult, valueName);

        RegCloseKey(phkResult);
    }

    if (status != nullptr)
        *status = _status;

    return _status == ERROR_SUCCESS;
}

#endif
