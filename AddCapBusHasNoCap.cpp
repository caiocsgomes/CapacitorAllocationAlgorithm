#include "stdafx.h"
#include "AddCapBusHasNoCap.h"
#include "Circuit.h"
#include "Bus.h"


AddCapBusWithNoCap::AddCapBusWithNoCap()
{
}


AddCapBusWithNoCap::~AddCapBusWithNoCap()
{
}

bool AddCapBusWithNoCap::execute(std::vector<Solution*> &solutions, std::vector<Solution*> initialSolution, Circuit *pCirc) {
	Solution *currentSolution{ nullptr };
	float currentGain{ 0 }, maxGain{ 0 };
	bool hasFound{ false };

	for (int index = 0; index < 20; index++) {

		//If the reactive part is negative and if the bus has a capacitor, the algorithm cannot place more capacitors
		if (initialSolution[index]->pBus->power[0].imag() < 0 || initialSolution[index]->pBus->numberOfCap != 0) continue;

		currentGain = this->calculateGainAdding(pCirc, initialSolution[index]->pBus);

		if (currentGain > maxGain) {
			maxGain = currentGain;
			currentSolution = initialSolution[index];
		}
	}

	if (maxGain > 0) {
		this->allocateCapacitor(currentSolution->pBus);
		solutions.push_back(currentSolution);
		hasFound = true;
	}

	return hasFound;
}
