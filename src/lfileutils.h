#ifndef LFILEUTILS_H
#define LFILEUTILS_H

#include "lchars.h"
#include "lukqtglobal.h"

#include <QDateTime>
#include <QFileInfo>

class LUKQT_DECLSPEC LFileUtils
{
public:
    static void fixFileName(QString &path, const QChar &replacedChar = LChars::underscore);

    static bool isTextFile(const QString &path, int bytesToCheck = 32);

    static bool isTextFile(QFile &file, int bytesToCheck = 32);

    static QByteArray readTextFile(const QString &path,
                                   qint64 maxLinesCount = 0,
                                   qint64 maxLineLength = 0);

    static QByteArray readTextFile(QFile &file,
                                   qint64 maxLinesCount = 0,
                                   qint64 maxLineLength = 0);

    static bool makeEmptyDir(const QString &path);

    static bool removeDir(const QString &path, bool recursively = true);

    static void clearDir(const QString &path,
                         const QStringList &nameFilters,
                         const QStringList &exceptNameFilters,
                         const QDateTime &fromDateTime = QDateTime(),
                         const QDateTime &toDateTime = QDateTime(),
                         bool recursively = true);

    static void removeOldFiles(const QString &path, const QDateTime &fromDateTime, bool recursively = true);

    static bool copyDir(const QString &sourcePath,
                        const QString &destinationPath,
                        const QStringList &nameFilters = QStringList(),
                        const QStringList &exceptNameFilters = QStringList(),
                        bool overwrite = true);

    static bool copyFile(const QString &sourcePath, const QString &destinationPath, bool overwrite = true);

    static QString lastModifiedFilePath(const QString &dirPath,
                                        const QStringList &nameFilters = QStringList(),
                                        const bool recursively = true);

    static QFileInfo lastModifiedFileInfo(const QString &dirPath,
                                          const QStringList &nameFilters = QStringList(),
                                          const bool recursively = true);

    static QString fileExtensionWildcard(const QString &fileExtension);

private:
    static void clearDir(const QString &path,
                         const QStringList &nameFilters,
                         const QVector<QRegularExpression> &exceptNameFilterRegularExpressions,
                         const QDateTime &fromDateTime,
                         const QDateTime &toDateTime,
                         bool recursively);

    static bool copyDir(const QString &sourcePath,
                        const QString &destinationPath,
                        const QStringList &nameFilters,
                        const QVector<QRegularExpression> &exceptNameFilterRegularExpressions,
                        bool overwrite);

    static bool hasMatchToRegularExpressions(const QVector<QRegularExpression> &regularExpressions,
                                             const QString &string);

    // deprecated since 6.0
    static QRegularExpression wildcardToRegularExpression(const QString &pattern,
                                                          Qt::CaseSensitivity cs = Qt::CaseInsensitive);

    static QVector<QRegularExpression> wildcardsToRegularExpressions(const QStringList &patterns,
                                                                     Qt::CaseSensitivity cs = Qt::CaseInsensitive);
};

#endif // LFILEUTILS_H
