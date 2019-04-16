#include "stdafx.h"
#include "Circuit.h"
#include "Bus.h"
#include "Flow.h"
#include "ReconfigureSolutions.h"
#include "const.h"


ReconfigureSolutions::ReconfigureSolutions()
{
}


ReconfigureSolutions::~ReconfigureSolutions()
{
}

std::vector<Solution*> ReconfigureSolutions::execute(std::vector<Solution*> solutions, Circuit *pCirc) {
	
	Solution *currentSolution{ nullptr };
	std::vector<Solution*> vectorOfSolutions;
	float currentGain{ 0 }, maxGain{ 0 };

	do {

		maxGain = 0;
		//We will take the 10 best busses to add the maximum number of capacitors
		//Capacitors will be added until a negative gain is reached
		//TODO take this magic number and add to const.h
		for (int index = 0; index < analysys::numberOfBussesAnalyzed; index++) {

			//If the reactive part is negative, the algorithm cannot place more capacitors
			if (solutions[index]->pBus->power[0].imag() < 0) continue;

			currentGain = this->calculateGainAdding(pCirc, solutions[index]->pBus);
			
			if (currentGain > maxGain) {
				maxGain = currentGain;
				currentSolution = solutions[index];
			}
		}

		if (maxGain > 0) {
			this->allocateCapacitor(currentSolution->pBus);
			vectorOfSolutions.push_back(currentSolution);
			currentSolution = nullptr;
		}
	} while (maxGain > 0);

	//TODO delete the solutions not used
	
	return vectorOfSolutions;
}
