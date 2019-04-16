#include "stdafx.h"
#include <algorithm>
#include "Bus.h"
#include "RemoveCap.h"

RemoveCap::RemoveCap() {}

RemoveCap::~RemoveCap() {}

bool RemoveCap::execute(std::vector<Solution*> &solutions, Circuit *pCirc) {

	Solution *currentSolution{ nullptr };
	float currentGain{ 0 }, maxGain{ 0 };
	bool hasFound{ false };
	
	for (Solution* solution : solutions) {

		currentGain = this->calculateGainRemoving(pCirc, solution->pBus);

		if (currentGain > maxGain) {
			maxGain = currentGain;
			currentSolution = solution;
		}
	}

	if (maxGain > 0) {
		this->removeCapacitor(currentSolution->pBus);
		if(currentSolution->pBus->numberOfCap == 0)
			solutions.erase(std::remove(solutions.begin(), solutions.end(), currentSolution), solutions.end());
		hasFound = true;
	}

	return hasFound;
}