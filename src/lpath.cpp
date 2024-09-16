#include "lpath.h"

#include "lchars.h"
#include "lmacros.h"



QString LPath::combine(const QString &path1, const QString &path2)
{
    return QSL("%1/%2").arg(path1, path2);
}



QString LPath::combine(const QString &path1, const QString &path2, const QString &path3)
{
    return QSL("%1/%2/%3").arg(path1, path2, path3);
}



QString LPath::combine(const QString &path1, const QString &path2, const QString &path3, const QString &path4)
{
    return QSL("%1/%2/%3/%4").arg(path1, path2, path3, path4);
}



QString LPath::combineExt(const QString &path, const QString &extension)
{
    return QSL("%1.%2").arg(path, extension);
}



QString LPath::combineExt(const QString &path1, const QString &path2, const QString &extension)
{
    return QSL("%1/%2.%3").arg(path1, path2, extension);
}



QString LPath::combineExt(const QString &path1, const QString &path2, const QString &path3, const QString &extension)
{
    return QSL("%1/%2/%3.%4").arg(path1, path2, path3, extension);
}



QString LPath::up(const QString &path, const int steps)
{
    if (steps < 0)
        return QString();

    return path.section(LChars::slash, 0, -steps - 1);
}



QString LPath::last(const QString &path, const int count)
{
    if (count <= 0)
        return QString();

    return path.section(LChars::slash, -count, -1);
}
