#ifndef LPATH_H
#define LPATH_H

#include "lukqtglobal.h"

#include <QString>

class LUKQT_DECLSPEC LPath
{
public:
    static QString combine(const QString &path1, const QString &path2);

    static QString combine(const QString &path1, const QString &path2, const QString &path3);

    static QString combine(const QString &path1, const QString &path2, const QString &path3, const QString &path4);

    static QString combineExt(const QString &path, const QString &extension);

    static QString combineExt(const QString &path1, const QString &path2, const QString &extension);

    static QString combineExt(const QString &path1,
                              const QString &path2,
                              const QString &path3,
                              const QString &extension);

    static QString up(const QString &path, int steps = 1);

    static QString last(const QString &path, int count = 1);
};

#endif // LPATH_H
