#pragma once
#include <vector>

class Bus;
class Circuit;

struct Solution {
	Bus	  *pBus;
	float gain;
};

class Metaheuristic
{
public:
	Metaheuristic();
	~Metaheuristic();
protected:
	Circuit * referenceCircuit;
	//std::vector<Solution*> vectorOfSolutions;
	float objectiveFunction(float lightLoadHours, float mediumLoadHours, float havyLoadHours);
	std::vector<Solution*> & quicksort(std::vector<Solution*> & solutions);
	int quicksortPartition(std::vector<Solution*> &solutions, int left, int right);
	void allocateCapacitor(Bus *pBus);
	void removeCapacitor(Bus *pBus);
	float calculateGainAdding(Circuit *pCirc, Bus *pBus);
	float calculateGainRemoving(Circuit *pCirc, Bus *pBus);
	static std::vector<Bus*> solutionBusses;
};

