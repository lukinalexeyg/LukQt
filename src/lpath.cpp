#include "lpath.h"

#include "lstringutils.h"



QString LPath::combine(const QString &path1, const QString &path2)
{
    return LStringUtils::concatStrings(path1, LChars::slash, path2);
}



QString LPath::combine(const QString &path1, const QString &path2, const QString &path3)
{
    return LStringUtils::concatStrings(path1, LChars::slash, path2, LChars::slash, path3);
}



QString LPath::combine(const QString &path1, const QString &path2, const QString &path3, const QString &path4)
{
    return LStringUtils::concatStrings(path1, LChars::slash, path2, LChars::slash, path3, LChars::slash, path4);
}



QString LPath::combineExt(const QString &path, const QString &extension)
{
    return LStringUtils::concatStrings(path, LChars::dot, extension);
}



QString LPath::combineExt(const QString &path1, const QString &path2, const QString &extension)
{
    return LStringUtils::concatStrings(path1, LChars::slash, path2, LChars::dot, extension);
}



QString LPath::combineExt(const QString &path1, const QString &path2, const QString &path3, const QString &extension)
{
    return LStringUtils::concatStrings(path1, LChars::slash, path2, LChars::slash, path3, LChars::dot, extension);
}



QString LPath::upDirPath(const QString &path)
{
    return path.section(LChars::slash, 0, -2);
}
