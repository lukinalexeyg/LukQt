#ifndef LWORKER_H
#define LWORKER_H

#include "lukqtglobal.h"

#include <QThread>

class LUKQT_DECLSPEC LWorker : public QObject
{
    Q_OBJECT

public:
    explicit LWorker(QObject *parent);

public:
    bool autoDelete() const { return m_autoDelete; }

    void setAutoDelete(const bool autoDelete) { m_autoDelete = autoDelete; }

    bool isThreadRunning() const;

    void startThread(const QString &name = QString());

    void stopThread();

    void terminateThread();

signals:
    void threadStarted();

    void threadFinished();

    void threadTerminated();

private:
    QObject *m_parent;
    QThread *m_thread;

    QAtomicInt m_autoDelete;
};

#endif // LWORKER_H
