#include "stdafx.h"
#include "AddCapBusHasCap.h"
#include "Bus.h"
#include "Circuit.h"
#include "Flow.h"
#include "const.h"

AddCapBusHasCap::AddCapBusHasCap()
{
}


AddCapBusHasCap::~AddCapBusHasCap()
{
}

bool AddCapBusHasCap::execute(std::vector<Solution*> &solutions, Circuit *pCirc) {

	Solution *currentSolution{ nullptr };
	float currentGain{ 0 }, maxGain{ 0 };
	bool hasFound{ false };
	
	for (Solution* solution : solutions) {

		//If the reactive part is negative, the algorithm cannot place more capacitors
		if (solution->pBus->power[0].imag() < 0) continue;

		currentGain = this->calculateGainAdding(pCirc, solution->pBus);

		if (currentGain > maxGain) {
			maxGain = currentGain;
			currentSolution = solution;
		}
	}

	if (maxGain > 0) {
		this->allocateCapacitor(currentSolution->pBus);
		solutions.push_back(currentSolution);
		bool hasFound = true;
	}
	
	return hasFound;
}
