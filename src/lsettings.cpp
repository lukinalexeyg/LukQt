#include "lsettings.h"

#include "lmacros.h"

#include <QFile>



LSettings::LSettings(const QString &organization, const QString &application, QObject *parent) :
    QSettings(organization, application, parent)
{
}



LSettings::LSettings(Scope scope, const QString &organization, const QString &application, QObject *parent) :
    QSettings(scope, organization, application, parent)
{
}



LSettings::LSettings(const Format format,
                     const Scope scope,
                     const QString &organization,
                     const QString &application,
                     QObject *parent) :
    QSettings(format, scope, organization, application, parent)
{
}



LSettings::LSettings(const QString &fileName, const QSettings::Format format, QObject *parent) :
    QSettings(fileName, format, parent)
{
}



LSettings::LSettings(QObject *parent) :
    QSettings(parent)
{
}



void LSettings::initBoolValue(const QString &name, bool &v)
{
    bool ok;
    int i = (int)v;
    i = value(name, i).toInt(&ok);

    if (!ok || (i != 0 && i != 1))
        i = (int)v;

    v = (bool)i;
    setValue(name, i);
}



bool LSettings::setIniSetting(const QString &filePath,
                              const QByteArray &name,
                              const QByteArray &value,
                              const qint64 maxLinesCount,
                              const qint64 maxLineLength)
{
    QFile file(filePath);

    if (!file.open(QFile::ReadWrite | QFile::Text))
        return false;

    qint64 l = 0;
    qint64 pos = 0;
    QByteArray buffer;

    while (!file.atEnd() && l < maxLinesCount) {
        QByteArray line = file.readLine(maxLineLength);

        if (pos == 0 && line.startsWith(name) && line.contains('=')) {
            pos = file.pos() - line.length() - 1;
            line = name + " = " + value + '\n';
        }

        if (pos > 0)
            buffer.append(line);

        ++l;
    }

    if (pos > 0) {
        file.seek(pos);
        file.write(buffer);
        file.close();
        return true;
    }

    file.close();
    return false;
}
