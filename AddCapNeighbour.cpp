#include "stdafx.h"
#include "AddCapNeighbour.h"
#include "Bus.h"


AddCapNeighbour::AddCapNeighbour()
{
}


AddCapNeighbour::~AddCapNeighbour()
{
}

bool AddCapNeighbour::execute(std::vector<Solution*> &solutions, Circuit *pCirc) {
	Solution *currentSolution{ nullptr };
	float currentGain{ 0 }, maxGain{ 0 };
	bool hasFound{ false };
	Solution *newSolution = new Solution;

	//First loop for pBus1 of busses
	for (Solution *pSolution : solutions) {

		if (pSolution->pBus->pBus1 != nullptr || pSolution->pBus->pBus1->numberOfCap != 0) {
			
			currentGain = this->calculateGainAdding(pCirc, pSolution->pBus->pBus1);

			if (currentGain > maxGain) {
				maxGain = currentGain;
				newSolution->pBus = pSolution->pBus->pBus1;
				newSolution->gain = maxGain;
				currentSolution = pSolution;
			}
		}

		if (pSolution->pBus->pBus2 != nullptr && pSolution->pBus->pBus2->numberOfCap != 0) {
			
			currentGain = this->calculateGainAdding(pCirc, pSolution->pBus->pBus1);

			if (currentGain > maxGain) {
				maxGain = currentGain;
				newSolution->pBus = pSolution->pBus->pBus2;
				newSolution->gain = maxGain;
				currentSolution = pSolution;
			}
		}

	}

	if (maxGain > 0) {
		this->allocateCapacitor(currentSolution->pBus);
		solutions.push_back(newSolution);
		hasFound = true;
	}
	else {
		delete newSolution;
	}

	return hasFound;
}