#pragma once
#include <thread>
#include <chrono>
#include <mutex>
class ThreadPool;
//===========================================================================================
class InterruptableThread{
public:
    InterruptableThread(ThreadPool* pool, int qindex);
    ~InterruptableThread();
    void startFunc(ThreadPool* pool, int qindex);
    void interrupt();
    void join();
    static bool checkInterrupted();

private:
    std::mutex m_defender;
    bool* m_pFlag;
    std::thread m_thread;
};

