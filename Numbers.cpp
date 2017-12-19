#include "Numbers.h"
#include <fstream>
#include <iostream>
#include <Windows.h>
Numbers::Numbers()
{
	std::cout << "Start Constructor" << std::endl;
	myPrinter = new std::thread(&Numbers::printer, this);
	for (size_t i = 0; i < MAX_THREADS; i++)
	{
		threads[i]=std::thread(&Numbers::worker, this);
	}
	myReader = new std::thread(&Numbers::reader, this);
	std::cout << "Finish Constructor" << std::endl;
}
Numbers::~Numbers()
{
	for (size_t i = 0; i < MAX_THREADS; i++)
		threads[i].join();
	isFinishedPrinter = true;
	myPrinter->join();
	myReader->join();
	std::cout << "Finish destructor" << std::endl;
}

void Numbers::reader()
{
	std::cout << "Start reader" << std::endl;
	int id=0;
	std::ifstream fin("in.txt");
//	std::ofstream fout;
	uint64 x;

	while(!fin.eof())
	{
		id++;
		{
			std::unique_lock<std::mutex> locker(mtxQIn);
			if (qIn.size() < MAX_THREADS)
			{
				fin >> x;
				qIn.push(Number(x, id));
				notifyWorker.notify_all();
			}
			else
			{
				notifyReader.wait(locker);
			}
		}
		Sleep(200);
	}
	isFinished = true;
	Sleep(1000);
	notifyWorker.notify_all();
	notifyPrinter.notify_all();
	fin.close();
	std::cout << "Finish reader" << std::endl;
}

void Numbers::printer()
{
	std::cout << "Start printer" << std::endl;
	bool wait = false;
	Number curNum;
	int printedId=0;
	{
		std::ofstream out;
		out.open("out.txt", std::ios_base::out);
		out << "Start printer" << std::endl;
		out.close();
	}
	while (true)
	{
		std::unique_lock<std::mutex> locker(mtxOut);
		if (Out.size() == 0)
			if (isFinishedPrinter)
				return;
			else
			{
				notifyPrinter.wait(locker);
				wait = true;
			}
		else
		{
			auto it = Out.begin();
			if ((*it).first == (printedId + 1))
			{
				curNum = (*it).second;
				Out.erase(printedId + 1);
				wait = false;
			}
		}
		if (!wait)
		{
			printedId++;
			std::ofstream out;
			out.open("out.txt", std::ios_base::app);
			out << "id = " << printedId << " Num = " << curNum.num << " Fact : " <<  curNum.getDecomposition() << std::endl;
			out.close();
		}
		Sleep(200);
	}
	std::cout << "Finish reader" << std::endl;
}

void Numbers::worker()
{
	std::cout << "Start worker" << std::endl;
	bool wait = false;
	Number curNum;
	while (true)
	{
		{
			std::unique_lock<std::mutex> locker(mtxQIn);
			if (qIn.size() == 0)
			{
				if (isFinished)
					return;
				else
				{
					wait = true;
					notifyWorker.wait(locker);
				}
			}
			else
			{
				curNum = qIn.front();
				qIn.pop();
				wait = false;
			}
		}
		if (!wait)
		{
			curNum.factorize();
			std::unique_lock<std::mutex> locker(mtxOut);
			Out[curNum.id] = curNum;
			notifyPrinter.notify_all();
		}
		Sleep(200);
	}
	std::cout << "Finish reader" << std::endl;
}