#include "stdafx.h"
#include "BruteForce.h"
#include "Circuit.h"
#include "Flow.h"
#include "Bus.h"
#include "const.h"


BruteForce::BruteForce() {}

BruteForce::~BruteForce(){
	//for (Solution *pSolution : this->vectorOfSolutions) delete pSolution;
}

std::vector<Solution*> BruteForce::execute(Circuit *pCirc) {
	
	float gain{ 0 };
	std::vector<Solution*> vectorOfSolutions;

	for (Bus *pBus : pCirc->busVector) {
		gain = this->calculateGainAdding(pCirc, pBus);
		
		if (gain > 0) {
			Solution *newSolution = new Solution();
			newSolution->pBus = pBus;
			newSolution->gain = gain;
			vectorOfSolutions.push_back(newSolution);
		}
	}

	this->quicksort(vectorOfSolutions);

	//We just want the 20 best busses
	//vectorOfSolutions.erase(vectorOfSolutions.begin() + 20, vectorOfSolutions.end());

	return vectorOfSolutions;
}
