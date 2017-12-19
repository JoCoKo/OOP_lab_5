#include "Number.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <math.h>

using namespace std;

template <typename T>
std::string toString(T val)
{
	std::ostringstream oss;
	oss << val;
	return oss.str();
}
Number::Number()
{
}
Number::Number(uint64 sourceNumber, int n, bool factor) : num(sourceNumber)
{
	id = n;
	if (factor)
	{
		factorize();
	}
}
void Number::factorize()
{
	// сюда складываем множители
	std::vector<uint64> dividers;
		// число, у которого осталось найти множители;
	uint64 curNum = num;
		// число, на которое пытаемся делить
	uint64 probe = 2;
	uint64 root = (uint64)(sqrt(num)+0.5);
	while (probe <= root)
	{
		if (curNum % probe != 0)
			// проверены все множители из [2; probe]
			probe++;
		else
		{
			// делим пока делится
			// cout << curNum << " " << probe << endl;
			curNum /= probe;
			root = (uint64)(sqrt(curNum) + 0.5);
			dividers.push_back(probe);
		}
	}
	dividers.push_back(curNum);

	// cout << dividers.size() << endl;
	for (size_t i = 0; i < dividers.size(); i++)
	{

		decomposition += toString(dividers[i])+" ";
	}
	alreadyFactored = true;
}

std::string Number::getDecomposition()
{
	if (!alreadyFactored)
	{
		factorize();
	}
	return decomposition;

}
;
