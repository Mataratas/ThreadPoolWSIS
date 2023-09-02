#include "RequestHandler.h"
//=============================================================================================
std::mutex coutLocker;
//-----------------------------------------------------------------------------------------------
void taskFunc(int id, int delay) { // функци€, выполн€юща€ задачу
	for (int i = 0; i < delay; i++) {
		if (InterruptableThread::checkInterrupted()) {
			std::unique_lock<std::mutex> l(coutLocker);
			std::cout << "task " << id << " was interrupted" << std::endl;
			return;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	std::unique_lock<std::mutex> l(coutLocker);
	std::cout << "task " << id << " made by thread_id " << std::this_thread::get_id() << std::endl; // выводим информацию о завершении
}
//-----------------------------------------------------------------------------------------------
int main(int argc, char** argv) {

	srand(0);
	RequestHandler rh;
	for (int i = 0; i < 20; i++)
		rh.pushRequest(taskFunc, i, 1 + rand() % 4);

	std::this_thread::sleep_for(std::chrono::seconds(1));
	rh.interruptPool();

	return 0;

}
//-----------------------------------------------------------------------------------------------
