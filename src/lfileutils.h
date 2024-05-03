#ifndef LFILEUTILS_H
#define LFILEUTILS_H

#include "lchars.h"
#include "lukqtglobal.h"

#include <QDateTime>
#include <QFileInfo>

class LUKQT_DECLSPEC LFileUtils
{
public:
    static QString concatPathes(const QString &path1, const QString &path2, const QString &path3 = QString());

    static QString concatFileName(const QString &name, const QString &extension);

    static QString appendFileNameToPath(const QString &path, const QString &fileName, const QString &fileExtension);

    static QString upAppDirPath();

    static void fixPath(QString &path, const QChar &replacedChar = LChars::underscore);

    static QByteArray readFile(const QString &filePath,
                               qint64 maxLinesCount = 0,
                               qint64 maxLineLength = 256);

    static bool makeEmptyDir(const QString &path);

    static bool removeDir(const QString &path);

    static bool clearDir(const QString &path,
                         const QStringList &nameFilters, const QStringList &exceptNameFilters,
                         const QDateTime &pastDateTime, const QDateTime &futureDateTime);

    static bool clearDirExcept(const QString &path, const QStringList &exceptNameFilters);

    static bool removeOldFiles(const QString &path, const QDateTime &pastDateTime);

    static bool copyDir(const QString &sourcePath,
                        const QString &destinationPath,
                        const QStringList &nameFilters = QStringList(),
                        const QStringList &exceptNameFilters = QStringList(),
                        bool overwrite = true);

    static bool copyDirExcept(const QString &sourcePath,
                              const QString &destinationPath,
                              const QStringList &exceptNameFilters);

    static QString lastModifiedFilePath(const QString &dirPath, const QStringList &nameFilters = QStringList());

    static QFileInfo lastModifiedFileInfo(const QString &dirPath, const QStringList &nameFilters = QStringList());

    static QString fileExtensionWildcard(const QString &fileExtension);

private:
    static bool copyDir(const QString &sourcePath,
                        const QString &destinationPath,
                        const QStringList &nameFilters,
                        const QVector<QRegularExpression> &exceptNameFilterRegularExpressions,
                        bool overwrite);

    static bool clearDir(const QString &path,
                         const QStringList &nameFilters,
                         const QVector<QRegularExpression> &exceptNameFilterRegularExpressions,
                         const QDateTime &pastDateTime,
                         const QDateTime &futureDateTime);

    static bool hasMatchToRegularExpressions(const QVector<QRegularExpression> &regularExpressions,
                                             const QString &string);

    // deprecated since 6.0
    static QRegularExpression wildcardToRegularExpression(const QString &pattern,
                                                          Qt::CaseSensitivity cs = Qt::CaseInsensitive);

    static QVector<QRegularExpression> wildcardsToRegularExpressions(const QStringList &patterns,
                                                                     Qt::CaseSensitivity cs = Qt::CaseInsensitive);
};

#endif // LFILEUTILS_H
