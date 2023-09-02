#pragma once
#include <thread>
#include <chrono>
#include <mutex>
#include "ThreadPool.h"
//===========================================================================================
thread_local bool thread_interrupt_flag = false;
//-------------------------------------------------------------------------------------------
class InterruptableThread{
public:
    InterruptableThread(ThreadPool* pool, int qindex);
    ~InterruptableThread();
    void startFunc(ThreadPool* pool, int qindex);
    void interrupt();
    void join();
    static bool checkInterrupted() {
        return thread_interrupt_flag;
    }

private:
    std::mutex m_defender;
    bool* m_pFlag;
    std::thread m_thread;
};

