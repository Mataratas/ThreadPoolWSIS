#include "InterruptableThread.h"
//===========================================================================================
InterruptableThread::InterruptableThread(ThreadPool* pool, int qindex):
    m_pFlag(nullptr),
    m_thread(&InterruptableThread::startFunc, this, taskFunc) 
{
}
//-------------------------------------------------------------------------------------------
InterruptableThread::~InterruptableThread() {
    if (m_thread.joinable())
        m_thread.join();
}
//-------------------------------------------------------------------------------------------
void InterruptableThread::startFunc(ThreadPool* pool, int qindex) {
    {
        std::lock_guard<std::mutex> l(m_defender);
        m_pFlag = &thread_interrupt_flag;  // инициализация указателя
    }
    pool->threadFunc(qindex); // вызвали то, что возможно будет прервано
    {
        std::lock_guard<std::mutex> l(m_defender);
        m_pFlag = nullptr; // защищаемся от обращений к несуществующей памяти
    }

}
//-------------------------------------------------------------------------------------------
void InterruptableThread::interrupt() {
    std::lock_guard<std::mutex> l(m_defender);
    if (m_pFlag) // защищаемся, чтоб не попасть, куда не нужно
        *m_pFlag = true;
}
//-------------------------------------------------------------------------------------------
void InterruptableThread::join() {
    if (m_thread.joinable())
        m_thread.join();
}

