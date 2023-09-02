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
        m_pFlag = &thread_interrupt_flag;  // ������������� ���������
    }
    pool->threadFunc(qindex); // ������� ��, ��� �������� ����� ��������
    {
        std::lock_guard<std::mutex> l(m_defender);
        m_pFlag = nullptr; // ���������� �� ��������� � �������������� ������
    }

}
//-------------------------------------------------------------------------------------------
void InterruptableThread::interrupt() {
    std::lock_guard<std::mutex> l(m_defender);
    if (m_pFlag) // ����������, ���� �� �������, ���� �� �����
        *m_pFlag = true;
}
//-------------------------------------------------------------------------------------------
void InterruptableThread::join() {
    if (m_thread.joinable())
        m_thread.join();
}

