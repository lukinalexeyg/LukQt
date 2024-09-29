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



bool LFileUtils::isTextFile(const QString &path, const int bytesToCheck)
{
    QFile file(path);

    if (!file.open(QFile::ReadOnly))
        return false;

    const bool isText = isTextFile(file, bytesToCheck);

    file.close();

    return isText;
}



bool LFileUtils::isTextFile(QFile &file, int bytesToCheck)
{
    const qint64 size = file.size();

    if (size == 0)
        return true;

    if (bytesToCheck <= 0)
        bytesToCheck = 32;

    const qint64 _size = qMin(S_CAST(qint64, bytesToCheck), size);
    const qint64 pos = file.pos();

    char *data = new char[_size];

    file.seek(0);
    const qint64 count = file.read(data, _size);
    file.seek(pos);

    bool isText = true;

    if (count > 0) {
        static const QVector<uchar> controlCharacterIndexes = {0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 12, 14, 15, 16, 17, 18,
                                                               19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 127};
        foreach_index_inc (i, count)
            if (controlCharacterIndexes.indexOf((uchar)data[i]) != -1) {
                isText = false;
                break;
            }
    }
    else
        isText = false;

    delete [] data;

    return isText;
}



QByteArray LFileUtils::readTextFile(const QString &path,
                                    const qint64 maxLinesCount,
                                    const qint64 maxLineLength)
{
    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text))
        return QByteArray();

    const QByteArray data = readTextFile(file, maxLinesCount, maxLineLength);

    file.close();

    return data;
}



QByteArray LFileUtils::readTextFile(QFile &file, const qint64 maxLinesCount, const qint64 maxLineLength)
{
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

    return data;
}



bool LFileUtils::makeEmptyDir(const QString &path)
{
    removeDir(path);
    return QDir().mkpath(path);
}



bool LFileUtils::removeDir(const QString &path, const bool recursively)
{
    QDir dir(path);

    if (recursively)
        return dir.removeRecursively();

    return dir.remove(path);
}



void LFileUtils::clearDir(const QString &path,
                          const QStringList &nameFilters, const QStringList &exceptNameFilters,
                          const QDateTime &fromDateTime, const QDateTime &toDateTime,
                          const bool recursively)
{
    clearDir(path,
             nameFilters,
             wildcardsToRegularExpressions(exceptNameFilters),
             fromDateTime,
             toDateTime,
             recursively);
}



void LFileUtils::removeOldFiles(const QString &path, const QDateTime &fromDateTime, const bool recursively)
{
    clearDir(path, QStringList(), QStringList(), fromDateTime, QDateTime(), recursively);
}



void LFileUtils::clearDir(const QString &path,
                          const QStringList &nameFilter,
                          const QVector<QRegularExpression> &exceptNameFilterRegularExpressions,
                          const QDateTime &fromDateTime,
                          const QDateTime &toDateTime,
                          const bool recursively)
{
    const QDir dir(path);

    if (!dir.exists())
        return;

    foreach_element_ref (fileInfo, dir.entryInfoList(nameFilter, QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        if (hasMatchToRegularExpressions(exceptNameFilterRegularExpressions, fileInfo.fileName()))
            continue;

        if (recursively && fileInfo.isDir())
            clearDir(fileInfo.absoluteFilePath(),
                     nameFilter,
                     exceptNameFilterRegularExpressions,
                     fromDateTime,
                     toDateTime,
                     recursively);

        else if ((fromDateTime.isNull() || fileInfo.lastModified() < fromDateTime)
                 && (toDateTime.isNull() || fileInfo.lastModified() > toDateTime)) {
            if (!QFile::remove(fileInfo.absoluteFilePath()))
                WARNING_LOG "error remove file:" << fileInfo.absoluteFilePath();
        }
    }

    dir.rmdir(path);
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

    if (!dir.exists())
        return false;

    bool ok = true;

    foreach_element_ref (d, dir.entryList(nameFilters, QDir::Dirs | QDir::NoDotAndDotDot)) {
        if (hasMatchToRegularExpressions(exceptNameFilterRegularExpressions, d))
            continue;

        const QString newPath = LPath::combine(destinationPath, d);

        if (!dir.mkpath(newPath)) {
            WARNING_LOG_E "error create directory:" << newPath;
            ok = false;
            continue;
        }

        if (!copyDir(LPath::combine(sourcePath, d),
                     newPath,
                     nameFilters,
                     exceptNameFilterRegularExpressions,
                     overwrite)) {
            WARNING_LOG_E "error copy directory:" << newPath;
            ok = false;
        }
    }

    foreach_element_ref (f, dir.entryList(QDir::Files | QDir::NoDotAndDotDot)) {
        if (hasMatchToRegularExpressions(exceptNameFilterRegularExpressions, f))
            continue;

        if (!copyFile(LPath::combine(sourcePath, f), LPath::combine(destinationPath, f), overwrite))
            ok = false;
    }

    return ok;
}



bool LFileUtils::copyFile(const QString &sourcePath, const QString &destinationPath, const bool overwrite)
{
    if (overwrite && QFile::exists(destinationPath) && !QFile::remove(destinationPath)) {
        WARNING_LOG_E "error remove overwritable file:" << destinationPath;
        return false;
    }

    if (!QFile::copy(sourcePath, destinationPath)) {
        WARNING_LOG_E "error copy file" << sourcePath << "to" << destinationPath;
        return false;
    }

    return true;
}



QString LFileUtils::lastModifiedFilePath(const QString &dirPath,
                                         const QStringList &nameFilters,
                                         const bool recursively)
{
    return lastModifiedFileInfo(dirPath, nameFilters, recursively).absoluteFilePath();
}



QFileInfo LFileUtils::lastModifiedFileInfo(const QString &dirPath,
                                           const QStringList &nameFilters,
                                           const bool recursively)
{
    QDir dir(dirPath);
    QFileInfo selectedFileInfo;

    if (recursively)
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
