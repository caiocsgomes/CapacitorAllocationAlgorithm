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
	static int numberOfCap(std::vector<Solution*> &solutions);
	static float calculateGainFinal(Circuit* pCircReference, Circuit* pCirc, std::vector<Solution*> &solutions);
	float calculateGainAdding(Circuit *pCirc, Bus *pBus);
	float calculateGainRemoving(Circuit *pCirc, Bus *pBus);
	static float objectiveFunctionFinal(float lightLoadHours, float mediumLoadHours, float havyLoadHours, std::vector<Solution*> &solutions);
	static float	calculateLosskWh(Circuit *pCirc);
	static float	calculateLosskW(Circuit *pCirc, int level);
	static std::vector<Bus*> solutionBusses;
};

