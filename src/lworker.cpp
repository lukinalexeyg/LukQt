#include "lworker.h"



LWorker::LWorker() :
    m_thread{nullptr},
    m_autoDeleteEnabled(true)
{
}



LWorker::~LWorker()
{
    m_autoDeleteEnabled = false;
    stopThread();
}



bool LWorker::isThreadRunning() const
{
    return m_thread != nullptr && m_thread->isRunning();
}



void LWorker::startThread(const QString &name)
{
    if (isThreadRunning())
        return;

    m_thread = new QThread;

    if (!name.isEmpty())
        m_thread->setObjectName(name);

    moveToThread(m_thread);

    connect(m_thread, &QThread::finished, this, [this] {
        emit threadFinished();
        m_thread->deleteLater();
        m_thread = nullptr;
        if (m_autoDeleteEnabled)
            deleteLater();
    });

    m_thread->start();

    emit threadStarted();
}



void LWorker::stopThread()
{
    if (!isThreadRunning())
        return;

    m_thread->quit();
    m_thread->wait();

    emit threadStopped();
}



void LWorker::terminateThread()
{
    if (!isThreadRunning())
        return;

    m_thread->terminate();
    m_thread->wait();

    emit threadTerminated();
}
