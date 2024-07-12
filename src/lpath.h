#ifndef LPATH_H
#define LPATH_H

#include "lchars.h"
#include "lukqtglobal.h"

class LUKQT_DECLSPEC LPath
{
public:
    static QString combine(const QString &path1, const QString &path2);

    static QString combine(const QString &path1, const QString &path2, const QString &path3);

    static QString combineExt(const QString &path, const QString &extension);

    static QString combineExt(const QString &path1, const QString &path2, const QString &extension);

    static QString upDirPath(const QString &path);

    static void fix(QString &path, const QChar &replacedChar = LChars::underscore);
};

#endif // LPATH_H
