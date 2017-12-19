#pragma once
#include <string>
#define uint64 unsigned __int64

class Number
{
public:
	Number();
	Number(uint64 sourceNumber, int id, bool factor = false);
	void factorize();
	std::string getDecomposition();
	int id;
	uint64 num;
private: 
	bool alreadyFactored=false;
	std::string decomposition="";
};