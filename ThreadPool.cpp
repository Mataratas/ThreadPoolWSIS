#include "ThreadPool.h"
//==================================================================
ThreadPool::ThreadPool() :
	m_thread_count(std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 4),
	m_thread_queues(m_thread_count)
{
#ifdef _DEBUG
	std::cout<<"m_thread_queues.size = " << m_thread_queues.size() << std::endl;
#endif // _DEBUG
}
//------------------------------------------------------------------
ThreadPool::~ThreadPool() {
	for (size_t i = 0; i < m_thread_count; i++)
		delete m_threads[i];

}
//------------------------------------------------------------------
void ThreadPool::start() {
	for (size_t i = 0; i < m_thread_count; i++){
		m_threads.push_back(new InterruptableThread(this, i)); 
	}
}
//------------------------------------------------------------------
void ThreadPool::stop() {
	for (size_t i = 0; i < m_thread_count; i++) {
		task_type empty_task;
		m_thread_queues[i].push(empty_task);
	}
	m_event_holder.notify_all();
	for (auto& t : m_threads)
		t->join();
}
//------------------------------------------------------------------
void ThreadPool::interrupt() {
	for (auto& t : m_threads) {
		t->interrupt();
	}
}

void ThreadPool::push_task(FuncType f, int id, int arg) {
	int queue_to_push = m_index++ % m_thread_count; // ��������� ������ �������, ���� ������� ������
	task_type new_task([=] {f(id, arg); }); // ��������� �������
	m_thread_queues[queue_to_push].push(new_task); // ������ � �������
	m_event_holder.notify_one();			// ��������� ��������� ����� � ����� ������
}
//------------------------------------------------------------------
void ThreadPool::threadFunc(int qindex) {
	while (true) {
		if (InterruptableThread::checkInterrupted()) {
			std::cout << "thread was interrupted" << std::endl;
			return;
		}
		task_type task_to_do;
		bool res{ false };
		int i{ 0 };

		for (; i < m_thread_count; i++) { // ������� ������ ������� ������ �� ����� �������, ������� �� �����				
			if (res = m_thread_queues[(qindex + i) % m_thread_count].fast_pop(task_to_do)) 
				break;
		}
		if (!res) //���� �� ����� �������� ����������� ��������� �������
			m_thread_queues[qindex].pop(task_to_do);
		else if(!task_to_do) //���� �������� ������ ������, �� ��� ������� ��������� ������� � ��� ��� �� �� ������� � ������� ������, �� ����� �� �� �����, ����� �������� � �������
			m_thread_queues[(qindex + i) % m_thread_count].push(task_to_do);

		if (!task_to_do)
			return;
			
		task_to_do();// ��������� ������
	}
}
//------------------------------------------------------------------


