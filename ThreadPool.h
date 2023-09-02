#pragma once
#include<iostream>
#include "BlockedQueue.h"
//=================================================================================
class InterruptableThread;

void taskFunc(int id, int delay);

typedef std::function<void()> task_type;

typedef void(*FuncType)(int, int);
//=================================================================================
class ThreadPool{
public:
    ThreadPool();
    ~ThreadPool();
    void start();
    void stop();
    void interrupt();
    void push_task(FuncType f, int id, int arg);
    void threadFunc(int);
private:    
    int m_thread_count;          // количество потоков (эта запись должна располагаться выше m_thread_queues!!!)
    int m_index;                 // индекс потока для равномерного распределения задач
    std::mutex m_locker;                // поддержка синхронизации очереди        
    std::vector<BlockedQueue<task_type>> m_thread_queues; // очереди задач по потокам
    std::condition_variable m_event_holder; // для синхронизации работы потоков    
    std::vector<InterruptableThread*> m_threads; // потоки   
 };

