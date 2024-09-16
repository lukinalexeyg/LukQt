#include "lfileutils.h"

#include "llog.h"
#include "lmacros.h"
#include "lpath.h"

#include <QDir>
#include <QRegularExpression>



void LFileUtils::fixFileName(QString &name, const QChar &replacedChar)
{
#ifdef Q_OS_WIN
    const QString chars("<>:\"/\\|?*");
#else
    const QString chars("/");
#endif

    std::replace_if(name.begin(), name.end(), [&chars](const QChar &c) {
        return chars.contains(c);
    }, replacedChar);
}



QByteArray LFileUtils::readTextFile(const QString &filePath,
                                    const qint64 maxLinesCount,
                                    const qint64 maxLineLength)
{
    QFile file(filePath);

    if (!file.open(QFile::ReadOnly | QFile::Text))
        return QByteArray();

    QByteArray data;

    if (maxLinesCount <= 0 && maxLineLength <= 0)
        data = file.readAll();

    else {
        qint64 l = 0;

        while (!file.atEnd() && l < maxLinesCount) {
            QByteArray line = file.readLine(maxLineLength);

            if (!line.endsWith('\n'))
                line.append('\n');

            data += line;
            ++l;
        }
    }

    file.close();

    return data;
}



bool LFileUtils::makeEmptyDir(const QString &path)
{
    removeDir(path);
    return QDir().mkpath(path);
}



bool LFileUtils::removeDir(const QString &path)
{
    QDir dir(path);
    return dir.removeRecursively();
}



bool LFileUtils::clearDir(const QString &path,
                          const QStringList &nameFilters, const QStringList &exceptNameFilters,
                          const QDateTime &pastDateTime, const QDateTime &futureDateTime)
{
    return clearDir(path, nameFilters, wildcardsToRegularExpressions(exceptNameFilters), pastDateTime, futureDateTime);
}



bool LFileUtils::clearDir(const QString &path,
                          const QStringList &nameFilter,
                          const QVector<QRegularExpression> &exceptNameFilterRegularExpressions,
                          const QDateTime &pastDateTime,
                          const QDateTime &futureDateTime)
{
    bool removed = true;
    const QDir dir(path);

    foreach_element_ref (fileInfo, dir.entryInfoList(nameFilter, QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        if (hasMatchToRegularExpressions(exceptNameFilterRegularExpressions, fileInfo.fileName()))
            continue;

        if (fileInfo.isDir()) {
            if (!clearDir(fileInfo.absoluteFilePath(),
                          nameFilter,
                          exceptNameFilterRegularExpressions,
                          pastDateTime,
                          futureDateTime))
                removed = false;
        }
        else if ((pastDateTime.isNull() || fileInfo.lastModified() < pastDateTime)
                 && (futureDateTime.isNull() || fileInfo.lastModified() > futureDateTime)) {
            if (!QFile::remove(fileInfo.absoluteFilePath())) {
                removed = false;
                WARNING_LOG "error remove file:" << fileInfo.absoluteFilePath();
            }
        }
    }

    if (dir.rmdir(path))
        DEBUG_LOG "dir removed:" << path;

    return removed;
}



bool LFileUtils::clearDirExcept(const QString &path, const QStringList &exceptNameFilter)
{
    const bool cleared = clearDir(path, QStringList(), exceptNameFilter, QDateTime(), QDateTime());

    DEBUG_LOG "finished:" << path;

    return cleared;
}



bool LFileUtils::removeOldFiles(const QString &path, const QDateTime &pastDateTime)
{
    const bool cleared = clearDir(path, QStringList(), QStringList(), pastDateTime, QDateTime());

    if (cleared)
        DEBUG_LOG "cleared:" << path;
    else
        WARNING_LOG "error clear:" << path;

    return cleared;
}



bool LFileUtils::copyDir(const QString &sourcePath, const QString &destinationPath,
                         const QStringList &nameFilters, const QStringList &exceptNameFilters,
                         const bool overwrite)
{
    return copyDir(sourcePath,
                   destinationPath,
                   nameFilters,
                   wildcardsToRegularExpressions(exceptNameFilters),
                   overwrite);
}



bool LFileUtils::copyDir(const QString &sourcePath,
                         const QString &destinationPath,
                         const QStringList &nameFilters,
                         const QVector<QRegularExpression> &exceptNameFilterRegularExpressions,
                         const bool overwrite)
{
    QDir dir(sourcePath);

    if (!dir.exists()) {
        WARNING_LOG "source directory not found:" << sourcePath;
        return false;
    }

    bool ok = true;

    foreach_element_ref (d, dir.entryList(nameFilters, QDir::Dirs | QDir::NoDotAndDotDot)) {
        if (hasMatchToRegularExpressions(exceptNameFilterRegularExpressions, d))
            continue;

        const QString newPath = LPath::combine(destinationPath, d);

        if (dir.mkpath(newPath)) {
            if (!copyDir(LPath::combine(sourcePath, d),
                         newPath,
                         nameFilters,
                         exceptNameFilterRegularExpressions,
                         overwrite))
                ok = false;
        }
        else {
            WARNING_LOG_E "create destination directory error:" << newPath;
            ok = false;
        }
    }

    foreach_element_ref (f, dir.entryList(QDir::Files | QDir::NoDotAndDotDot)) {
        if (hasMatchToRegularExpressions(exceptNameFilterRegularExpressions, f))
            continue;

        const QString newPath = LPath::combine(destinationPath, f);

        if (overwrite && QFile::exists(newPath) && !QFile::remove(newPath)) {
            WARNING_LOG_E "remove old file error:" << newPath;
            ok = false;
        }

        if (!QFile::copy(LPath::combine(sourcePath, f), newPath))
            ok = false;
    }

    return ok;
}



bool LFileUtils::copyDirExcept(const QString &sourcePath,
                               const QString &destinationPath,
                               const QStringList &exceptNameFilters)
{
    return copyDir(sourcePath, destinationPath, QStringList(), exceptNameFilters, true);
}



QFileInfo LFileUtils::lastModifiedFileInfo(const QString &dirPath, const QStringList &nameFilters)
{
    QDir dir(dirPath);
    QFileInfo selectedFileInfo;

    foreach_element_ref (fileInfo, dir.entryInfoList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time)) {
        const QFileInfo _fileInfo = lastModifiedFileInfo(fileInfo.absoluteFilePath(), nameFilters);
        if (_fileInfo.lastModified() > selectedFileInfo.lastModified())
            selectedFileInfo = _fileInfo;
    }

    const QFileInfoList fileInfoList = dir.entryInfoList(nameFilters, QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
    if (!fileInfoList.isEmpty()) {
        const QFileInfo fileInfo = fileInfoList.first();
        if (fileInfo.lastModified() > selectedFileInfo.lastModified())
            selectedFileInfo = fileInfo;
    }

    return selectedFileInfo;
}



QString LFileUtils::lastModifiedFilePath(const QString &dirPath, const QStringList &nameFilters)
{
    return lastModifiedFileInfo(dirPath, nameFilters).absoluteFilePath();
}



QString LFileUtils::fileExtensionWildcard(const QString &fileExtension)
{
    return QSL("*.%1").arg(fileExtension);
}



bool LFileUtils::hasMatchToRegularExpressions(const QVector<QRegularExpression> &regularExpressions,
                                              const QString &string)
{
    foreach_element_const_ref (regularExpression, regularExpressions)
        if (regularExpression.match(string).hasMatch())
            return true;

    return false;
}



QRegularExpression LFileUtils::wildcardToRegularExpression(const QString &pattern, const Qt::CaseSensitivity cs)
{
    const QRegularExpression::PatternOption options = (cs == Qt::CaseSensitive
                                                           ? QRegularExpression::NoPatternOption
                                                           : QRegularExpression::CaseInsensitiveOption);

    return QRegularExpression(QRegularExpression::wildcardToRegularExpression(pattern), options);
}



QVector<QRegularExpression> LFileUtils::wildcardsToRegularExpressions(const QStringList &patterns,
                                                                      const Qt::CaseSensitivity cs)
{
    QVector<QRegularExpression> regularExpressions;
    regularExpressions.reserve(patterns.size());

    foreach_element_const_ref (pattern, patterns)
        regularExpressions.append(wildcardToRegularExpression(pattern, cs));

    return regularExpressions;
}
