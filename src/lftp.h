#ifndef LFTP_H
#define LFTP_H

#include "lukqtglobal.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

typedef QSharedPointer<QNetworkReply> NetworkReplyPtr;

class LUKQT_DECLSPEC LFtp : public QObject
{
    Q_OBJECT

public:
    explicit LFtp(QObject *parent = nullptr);

public:
    QString origin() const                      { return m_origin; }

    void setOrigin(const QString &origin)       { m_origin = origin; }

    QString userName() const                    { return m_userName; }

    void setUserName(const QString &userName)   { m_userName = userName; }

    QString password() const                    { return m_password; }

    void setPassword(const QString &password)   { m_password = password; }

    void setGetTimeout(const int timeout)       { m_getTimeout = timeout; }

    void setPutTimeout(const int timeout)       { m_putTimeout = timeout; }

    NetworkReplyPtr get(const QString &path);

    NetworkReplyPtr put(const QString &path, const QByteArray &data);

signals:
    void progressChanged(NetworkReplyPtr networkReply, qreal progress);

    void getProgressChanged(NetworkReplyPtr networkReply, qreal progress);

    void putProgressChanged(NetworkReplyPtr networkReply, qreal progress);

    void finished(NetworkReplyPtr networkReply);

    void getFinished(NetworkReplyPtr networkReply);

    void putFinished(NetworkReplyPtr networkReply);

    void errorOccured(NetworkReplyPtr networkReply);

private:
    QNetworkAccessManager *m_networkAccessManager;
    QList<NetworkReplyPtr> m_replies;

    QString m_origin;
    QString m_userName;
    QString m_password;

    int m_getTimeout;
    int m_putTimeout;

private:
    QNetworkRequest networkRequest(const QString &path) const;

    void setErrorHandlers(QNetworkReply *networkReply, int timeout);

    void onGetProgressChanged(qint64 bytesSent, qint64 bytesTotal);

    void onPutProgressChanged(qint64 bytesSent, qint64 bytesTotal);

    qreal progress(qint64 bytesSent, qint64 bytesTotal) const;

    void onGetFinished();

    void onPutFinished();

    void onError(QNetworkReply::NetworkError error);

    NetworkReplyPtr getNetworkReplyPtr(QNetworkReply *networkReply, bool remove);
};

#endif // LFTP_H
