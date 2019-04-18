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
	static void log(std::vector<Solution*> &solutions, Circuit* pCircReference, Circuit* pCirc);
protected:
	Circuit * referenceCircuit;
	//std::vector<Solution*> vectorOfSolutions;
	static float objectiveFunction(float lightLoadHours, float mediumLoadHours, float havyLoadHours);
	std::vector<Solution*> & quicksort(std::vector<Solution*> & solutions);
	int quicksortPartition(std::vector<Solution*> &solutions, int left, int right);
	void allocateCapacitor(Bus *pBus);
	void removeCapacitor(Bus *pBus);
	static float calculateGain(Circuit* pCircReference, Circuit* pCirc);
	float calculateGainAdding(Circuit *pCirc, Bus *pBus);
	float calculateGainRemoving(Circuit *pCirc, Bus *pBus);
	static std::vector<Bus*> solutionBusses;
};

