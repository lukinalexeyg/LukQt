#include "lworker.h"



LWorker::LWorker(QObject *threadParent) :
    m_thread{nullptr},
    m_threadParent{threadParent},
    m_autoDeleteEnabled(true)
{
}



bool LWorker::isThreadRunning() const
{
    return m_thread != nullptr && m_thread->isRunning();
}



void LWorker::startThread(const QString &name)
{
    if (m_thread != nullptr)
        return;

    m_thread = new QThread(m_threadParent);

    if (!name.isEmpty())
        m_thread->setObjectName(name);

    moveToThread(m_thread);

    connect(m_thread, &QThread::finished, this, [this] {
        emit threadFinished();
        if (m_autoDeleteEnabled)
            deleteLater();
    });

    m_thread->start();

    emit threadStarted();
}



void LWorker::stopThread()
{
    if (m_thread == nullptr)
        return;

    m_thread->quit();
    m_thread->wait();

    emit threadStopped();
}



void LWorker::terminateThread()
{
    if (m_thread == nullptr)
        return;

    m_thread->terminate();
    m_thread->wait();

    emit threadTerminated();
}
