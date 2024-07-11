#include "lftp.h"

#include "lftp.h"
#include "llog.h"
#include "lmacros.h"
#include "lpath.h"

#include <QTimer>



LFtp::LFtp(QObject *parent) :
    QObject(parent),
    m_networkAccessManager(new QNetworkAccessManager(this)),
    m_getTimeout(-1),
    m_putTimeout(-1)
{
}



NetworkReplyPtr LFtp::get(const QString &path)
{
    QNetworkReply *networkReply = m_networkAccessManager->get(networkRequest(path));
    NetworkReplyPtr networkReplyPtr = NetworkReplyPtr(networkReply);

    m_replies.append(networkReplyPtr);

    connect(networkReply, &QNetworkReply::downloadProgress, this, &LFtp::onGetProgressChanged);
    connect(networkReply, &QNetworkReply::finished, this, &LFtp::onGetFinished);

    setErrorHandlers(networkReply, m_getTimeout);

    return networkReplyPtr;
}



NetworkReplyPtr LFtp::put(const QString &path, const QByteArray &data)
{
    QNetworkReply *networkReply = m_networkAccessManager->put(networkRequest(path), data);
    NetworkReplyPtr networkReplyPtr = NetworkReplyPtr(networkReply);

    m_replies.append(networkReplyPtr);

    connect(networkReply, &QNetworkReply::downloadProgress, this, &LFtp::onPutProgressChanged);
    connect(networkReply, &QNetworkReply::finished, this, &LFtp::onPutFinished);

    setErrorHandlers(networkReply, m_putTimeout);

    return networkReplyPtr;
}



QNetworkRequest LFtp::networkRequest(const QString &path) const
{
    QUrl url;

    const QString urlString = m_origin.isEmpty() ? path : LPath::combine(m_origin, path);

    url.setUrl(urlString);
    url.setUserName(m_userName);
    url.setPassword(m_password);

    return QNetworkRequest(url);
}



void LFtp::setErrorHandlers(QNetworkReply *networkReply, const int timeout)
{
    connect(networkReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &LFtp::onError);

    // deprecated since 5.15.0, use QNetworkAccessManager::transferTimeout() instead
    if (timeout > 0)
        QTimer::singleShot(timeout, this, [this, networkReply]() {
            NetworkReplyPtr networkReplyPtr = getNetworkReplyPtr(networkReply, false);
            if (!networkReplyPtr.isNull()) {
                WARNING_LOG "request timeout:" << networkReplyPtr->operation() << networkReplyPtr->url().path();
                networkReplyPtr->abort();
            }
        });
}



void LFtp::onGetProgressChanged(const qint64 bytesSent, const qint64 bytesTotal)
{
    SET_QOBJECT(QNetworkReply*, networkReply, sender());

    NetworkReplyPtr networkReplyPtr = getNetworkReplyPtr(networkReply, false);

    if (!networkReplyPtr.isNull()) {
        const qreal progress = this->progress(bytesSent, bytesTotal);
        emit progressChanged(networkReplyPtr, progress);
        emit getProgressChanged(networkReplyPtr, progress);
    }
}



void LFtp::onPutProgressChanged(const qint64 bytesSent, const qint64 bytesTotal)
{
    SET_QOBJECT(QNetworkReply*, networkReply, sender());

    NetworkReplyPtr networkReplyPtr = getNetworkReplyPtr(networkReply, false);

    if (!networkReplyPtr.isNull()) {
        const qreal progress = this->progress(bytesSent, bytesTotal);
        emit progressChanged(networkReplyPtr, progress);
        emit putProgressChanged(networkReplyPtr, progress);
    }
}



qreal LFtp::progress(const qint64 bytesSent, const qint64 bytesTotal) const
{
    return bytesTotal > 0 ?  100.0 * S_CAST_REAL(bytesSent)/S_CAST_REAL(bytesTotal) : 0.0;
}



void LFtp::onGetFinished()
{    
    SET_QOBJECT(QNetworkReply*, networkReply, sender());

    NetworkReplyPtr networkReplyPtr = getNetworkReplyPtr(networkReply, true);

    if (!networkReplyPtr.isNull()) {
        emit finished(networkReplyPtr);
        emit getFinished(networkReplyPtr);
    }
}



void LFtp::onPutFinished()
{
    SET_QOBJECT(QNetworkReply*, networkReply, sender());

    NetworkReplyPtr networkReplyPtr = getNetworkReplyPtr(networkReply, true);

    if (!networkReplyPtr.isNull()) {
        emit finished(networkReplyPtr);
        emit putFinished(networkReplyPtr);
    }
}



void LFtp::onError(const QNetworkReply::NetworkError error)
{
    Q_UNUSED(error)

    SET_QOBJECT(QNetworkReply*, networkReply, sender());

    NetworkReplyPtr networkReplyPtr = getNetworkReplyPtr(networkReply, false);

    if (!networkReplyPtr.isNull()) {
        WARNING_LOG networkReplyPtr->operation() << networkReplyPtr->url().path() << networkReplyPtr->error();
        emit this->errorOccured(networkReplyPtr);
    }
}



NetworkReplyPtr LFtp::getNetworkReplyPtr(QNetworkReply *networkReply, const bool remove)
{
    foreach_element_const (r, m_replies)
        if (r.get() == networkReply) {
            if (remove)
                m_replies.removeOne(r);
            return r;
        }

    return NetworkReplyPtr(nullptr);
}
