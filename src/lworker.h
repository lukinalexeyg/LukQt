#ifndef LWORKER_H
#define LWORKER_H

#include "lukqtglobal.h"

#include <QThread>

class LUKQT_DECLSPEC LWorker : public QObject
{
    Q_OBJECT

public:
    explicit LWorker();

    ~LWorker();

public:
    bool autoDeleteEnabled() const { return m_autoDeleteEnabled; }

    void setAutoDeleteEnabled(const bool enabled) { m_autoDeleteEnabled = enabled; }

    bool isThreadRunning() const;

    void startThread(const QString &name = QString());

    void stopThread();

    void terminateThread();

signals:
    void threadStarted();

    void threadStopped();

    void threadFinished();

    void threadTerminated();

private:
    QThread *m_thread;

    QAtomicInt m_autoDeleteEnabled;
};

#endif // LWORKER_H
