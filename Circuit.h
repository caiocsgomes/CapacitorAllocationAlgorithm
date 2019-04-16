#pragma once
#include <vector>

class Bus;
class Branch;

class Circuit
{
public:
	Circuit();
	~Circuit();
	Bus* getBusById(int id);
	std::vector<Branch*> branchVector;
	std::vector<Bus*> busVector;
};

