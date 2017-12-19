#pragma once
#include <queue>
#include <condition_variable>
#include <map>
#include <mutex>
#include "Number.h"
#include <thread>
#include <array>

#define MAX_THREADS 8
#define MIN_THREADS 2

class Numbers
{
public:
	Numbers();
	~Numbers();
	void reader();
	void printer();
	void worker(size_t id);
	void voidMe();
private:
	std::queue<Number> qIn;
	std::map<int,Number> Out;
	std::mutex mtxQIn;
	std::mutex mtxOut;
	std::mutex log;
	mutable std::array<std::thread, MAX_THREADS> threads;
	std::thread *myPrinter;
	std::thread *myReader;
	bool isFinished=false;
	bool isFinishedPrinter = false;
	size_t numWorkers=0;
	std::condition_variable notifyWorker;
	std::condition_variable notifyReader;
	std::condition_variable notifyPrinter;
};

