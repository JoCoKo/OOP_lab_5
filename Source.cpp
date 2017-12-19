#include <iostream>
#include <fstream>
#include <thread>
#include "Number.h"
#include "Numbers.h"
#include "Windows.h"

using namespace std;

int main(int argc, char* argv[])
{
/*	Number* t1 = new Number(12);
	cout << "RESULT:" << t1->getDecomposition() << "#" << endl;
	if (argc >= 3)
	{
		uint64 curNum;
		ofstream fout(argv[2]);
		ifstream fin(argv[1]);

		while (!fin.eof())
		{
			fin >> curNum;
			cout << curNum << " ";
			Number* w1 = new Number(curNum);
			fout << w1->getDecomposition() << endl;
		}
		fout.close();
		fin.close();
	}
	else
	{
		cout << "Not enough params" << endl;
	};
	*/
	{
	    Numbers nums;
        nums.voidMe();
	}
	//Sleep(5000);
	//cout << std::thread::hardware_concurrency() << endl;
	system("pause");
	return 0;
}
