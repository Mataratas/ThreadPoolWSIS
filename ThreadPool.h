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
    int m_thread_count;          // ���������� ������� (��� ������ ������ ������������� ���� m_thread_queues!!!)
    int m_index;                 // ������ ������ ��� ������������ ������������� �����
    std::mutex m_locker;                // ��������� ������������� �������        
    std::vector<BlockedQueue<task_type>> m_thread_queues; // ������� ����� �� �������
    std::condition_variable m_event_holder; // ��� ������������� ������ �������    
    std::vector<InterruptableThread*> m_threads; // ������   
 };

