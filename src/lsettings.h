#ifndef LSETTINGS_H
#define LSETTINGS_H

#include "lukqtglobal.h"

#include <QSettings>

/*!
\brief QSettings wrapper
*/
class LUKQT_DECLSPEC LSettings : public QSettings
{
public:
    explicit LSettings(const QString &organization, const QString &application = QString(), QObject *parent = nullptr);

    LSettings(QSettings::Scope scope,
             const QString &organization,
             const QString &application = QString(),
             QObject *parent = nullptr);

    LSettings(QSettings::Format format,
             QSettings::Scope scope,
             const QString &organization,
             const QString &application = QString(),
             QObject *parent = nullptr);

    LSettings(const QString &fileName, QSettings::Format format, QObject *parent = nullptr);

    explicit LSettings(QObject *parent = nullptr);

public:
    /*!
    Initialize setting.
    \param name name of the setting
    \param value value of the setting
    \param allowedValues allowed values of the setting
    */
    template <typename T>
    void initValue(const QString &name, T &v, const QList<T> &allowedValues = QList<T>())
    {
        const QVariant a = value(name, v);
        v = a.value<T>();
        v = (allowedValues.isEmpty() || allowedValues.contains(v)) ? v : allowedValues.first();
        setValue(name, v);
    }

    /*!
    Initialize bool setting.
    \param name name of the setting
    \param value value of the setting
    */
    void initBoolValue(const QString &name, bool &v);

    /*!
    Initialize number setting.
    \param name name of the setting
    \param v value of the setting
    \param min minimal value of the setting
    \param max maximal value of the setting
    */
    template <typename T>
    void initNumberValue(const QString &name, T &v, T min, T max)
    {
        const QVariant a = value(name, v);
        v = a.value<T>();
        v = qBound(min, v, max);
        setValue(name, v);
    }

    static bool setIniSetting(const QString &filePath,
                              const QByteArray &name,
                              const QByteArray &value,
                              qint64 maxLinesCount = 65536,
                              qint64 maxLineLength = 256);
};

#endif // LSETTINGS_H
