#include "Numbers.h"
#include <fstream>
#include <iostream>
#include <Windows.h>

Numbers::Numbers()
{
    {
        std::unique_lock<std::mutex> locker(log);
        std::cout << "Start Constructor" << std::endl;
    }
	myPrinter = new std::thread(&Numbers::printer, this);
	for (size_t i = 0; i < MAX_THREADS; i++)
	{
		threads[i]=std::thread(&Numbers::worker, this, i + 1);
	}
	Sleep(501);
	myReader = new std::thread(&Numbers::reader, this);
    {
        std::unique_lock<std::mutex> locker(log);
        std::cout << "Finish Constructor" << std::endl;
    }
}
Numbers::~Numbers()
{
	for (size_t i = 0; i < MAX_THREADS; i++)
		threads[i].join();
	Sleep(201);
	isFinishedPrinter = true;
	notifyPrinter.notify_one();
	myPrinter->join();
	myReader->join();
    {
        std::unique_lock<std::mutex> locker(log);
        std::cout << "Finish Destructor" << std::endl;
    }
}

// ########################### READER ##########################
void Numbers::reader()
{
    {
        std::unique_lock<std::mutex> locker(log);
        std::cout << "Start reader" << std::endl;
    }
	int id=0;
	std::ifstream fin("in.txt");
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
				Sleep(301);
 				notifyWorker.notify_all();
               {
                    std::unique_lock<std::mutex> locker(log);
                    std::cout << "reader push in queue number = " << x << " with id = " << id << std::endl;
                }
			}
			else
			{
				notifyReader.wait(locker);
			}
		}
		Sleep(100);
	}
	isFinished = true;
	Sleep(501);
	notifyWorker.notify_all();
	notifyPrinter.notify_all();
	fin.close();
    {
        std::unique_lock<std::mutex> locker(log);
        std::cout << "Finish Reader" << std::endl;
    }
}

// ########################### PRINTER ##########################
void Numbers::printer()
{
    {
        std::unique_lock<std::mutex> locker(log);
        std::cout << "Start printer" << std::endl;
    }
//	std::cout << "" << std::endl;
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
				break;
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
    {
        std::unique_lock<std::mutex> locker(log);
        std::cout << "Finish Printer" << std::endl;
    }
}

// ########################### WORKER ##########################
void Numbers::worker(size_t myID)
{
    {
        std::unique_lock<std::mutex> locker(log);
        std::cout << "Start worker " << myID << std::endl;
    }
	bool wait = false;
	Number curNum;
	while (true)
	{
		{
			std::unique_lock<std::mutex> locker(mtxQIn);
			if (qIn.size() == 0)
			{
				if (isFinished)
					break;
				else
				{
					wait = true;
  					notifyWorker.wait(locker);
                  {
                        std::unique_lock<std::mutex> locker(log);
                        std::cout << "worker " << myID << " sleeping" << std::endl;
                  }
				}
			}
			else
			{
				curNum = qIn.front();
				qIn.pop();
				wait = false;
				notifyReader.notify_all();
			}
		}
		if (!wait)
		{
            {
                std::unique_lock<std::mutex> locker(log);
                std::cout << "worker " << myID << " calculating " << curNum.num << " with id = " << curNum.id << std::endl;
            }
			curNum.factorize();
			std::unique_lock<std::mutex> locker(mtxOut);
			Out[curNum.id] = curNum;
			notifyPrinter.notify_all();
		}
		Sleep(200);
	}
    {
        std::unique_lock<std::mutex> locker(log);
        std::cout << "Finish worker " << myID << std::endl;
    }
}

void Numbers::voidMe()
{
    {
        std::unique_lock<std::mutex> locker(log);
        std::cout << "Doing something" << std::endl;
    }
}

