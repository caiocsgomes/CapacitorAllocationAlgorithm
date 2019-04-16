#include "stdafx.h"
#include "stack"
#include "Bus.h"
#include "Circuit.h"
#include "Flow.h"
#include "Metaheuristic.h"
#include "const.h"

Metaheuristic::Metaheuristic() {}

Metaheuristic::~Metaheuristic() {}

//This function will calculate the gain in dollars with the capacitor of 300kVAr installation in the bus
float Metaheuristic::objectiveFunction(float lossGainLight, float lossGainMedium, float lossGainHeavy) {
	float profit = energy::pricekWh * (lossGainLight * general::lightLoadHours + lossGainMedium * general::mediumLoadHours + lossGainHeavy * general::heavyLoadHours) - (capacitor::price300kVAr + capacitor::price300kVArInstalation);
	return profit;
}

//As we just want the best solutions, we need to sort it based on gain
std::vector<Solution*> &Metaheuristic::quicksort(std::vector<Solution*> &solutions) {
	int start = 0;
	int end = solutions.size() - 1;
	int partitionIndex = 0;
	std::stack<int> stack;
	stack.push(start);
	stack.push(end);

	while (stack.size() > 0) {
		end = stack.top();
		stack.pop();
		start = stack.top();
		stack.pop();

		partitionIndex = quicksortPartition(solutions, start, end);

		if (partitionIndex + 1 < end) {
			stack.push(partitionIndex + 1);
			stack.push(end);
		}

		if (start < partitionIndex - 1) {
			stack.push(start);
			stack.push(partitionIndex - 1);
		}
	}
	
	return solutions;
}

int Metaheuristic::quicksortPartition(std::vector<Solution*> &solutions, int left, int right) {
	int partitionIndex = left;
	//The number wee need to compare is the loss from the last solution
	float pivot = solutions[right]->gain;
	Solution *auxVariable;

	for (int index = left; index < right; index++) {
		//>= will sort from the biggest to the smallest
		if (solutions[index]->gain >= pivot) {
			auxVariable = solutions[partitionIndex];
			solutions[partitionIndex] = solutions[index];
			solutions[index] = auxVariable;
			partitionIndex++;
		}
	}

	auxVariable = solutions[right];
	solutions[right] = solutions[partitionIndex];
	solutions[partitionIndex] = auxVariable;
	return partitionIndex;
}

void Metaheuristic::allocateCapacitor(Bus *pBus) {
	pBus->power->imag(pBus->power->imag() - 300);
	pBus->numberOfCap += 1;
}

void Metaheuristic::removeCapacitor(Bus *pBus) {
	pBus->power->imag(pBus->power->imag() + 300);
	pBus->numberOfCap -= 1;
}

float Metaheuristic::calculateGainAdding(Circuit *pCirc, Bus *pBus) {
	
	float lossGainLight{ 0 }, lossGainMedium{ 0 }, lossGainHeavy{ 0 };
	float lossLight{ 0 }, lossMedium{ 0 }, lossHeavy{ 0 };
	float lossLightCap{ 0 }, lossMediumCap{ 0 }, lossHeavyCap{ 0 };
	float gain{ 0 };
	
	Flow pFlow(pCirc, general::voltageReference, general::lossTolerance);

	//Flow for light load with no capacitor
	pFlow.level = loadLevel::light;
	pFlow.execute();
	lossLight = pFlow.loss;

	//Flow for medium load with no capacitor
	pFlow.level = loadLevel::medium;
	pFlow.execute();
	lossMedium = pFlow.loss;

	//Flow for light load with no capacitor
	pFlow.level = loadLevel::heavy;
	pFlow.execute();
	lossHeavy = pFlow.loss;

	this->allocateCapacitor(pBus);

	//Running flow again with capacitor added for light
	lossLight = pFlow.loss;
	pFlow.execute();
	lossLightCap = pFlow.loss;

	//Running flow again with capacitor added for medium
	pFlow.level = loadLevel::medium;
	pFlow.execute();
	lossMediumCap = pFlow.loss;

	//Running flow again with capacitor added for heavy
	pFlow.level = loadLevel::heavy;
	pFlow.execute();
	lossHeavyCap = pFlow.loss;

	this->removeCapacitor(pBus);

	//Change from W to kW
	lossGainLight  = (lossLight - lossLightCap) / 1000;
	lossGainMedium = (lossMedium - lossMediumCap) / 1000;
	lossGainHeavy  = (lossHeavy - lossHeavyCap) / 1000;

	gain = this->objectiveFunction(lossGainLight, lossGainMedium, lossGainHeavy);

	return gain;
}

float Metaheuristic::calculateGainRemoving(Circuit *pCirc, Bus *pBus) {

	float lossGainLight{ 0 }, lossGainMedium{ 0 }, lossGainHeavy{ 0 };
	float lossLight{ 0 }, lossMedium{ 0 }, lossHeavy{ 0 };
	float lossLightCap{ 0 }, lossMediumCap{ 0 }, lossHeavyCap{ 0 };
	float gain{ 0 };

	Flow pFlow(pCirc, general::voltageReference, general::lossTolerance);

	//Flow for light load with no capacitor
	pFlow.level = loadLevel::light;
	pFlow.execute();
	lossLight = pFlow.loss;

	//Flow for medium load with no capacitor
	pFlow.level = loadLevel::medium;
	pFlow.execute();
	lossMedium = pFlow.loss;

	//Flow for light load with no capacitor
	pFlow.level = loadLevel::heavy;
	pFlow.execute();
	lossHeavy = pFlow.loss;

	this->removeCapacitor(pBus);

	//Running flow again with capacitor added for light
	lossLight = pFlow.loss;
	pFlow.execute();
	lossLightCap = pFlow.loss;

	//Running flow again with capacitor added for medium
	pFlow.level = loadLevel::medium;
	pFlow.execute();
	lossMediumCap = pFlow.loss;

	//Running flow again with capacitor added for heavy
	pFlow.level = loadLevel::heavy;
	pFlow.execute();
	lossHeavyCap = pFlow.loss;

	this->allocateCapacitor(pBus);

	//Change from W to kW
	lossGainLight = (lossLight - lossLightCap) / 1000;
	lossGainMedium = (lossMedium - lossMediumCap) / 1000;
	lossGainHeavy = (lossHeavy - lossHeavyCap) / 1000;

	gain = this->objectiveFunction(lossGainLight, lossGainMedium, lossGainHeavy);

	return gain;
}