#include "stdafx.h"
#include "stack"
#include "Bus.h"
#include "Circuit.h"
#include "Flow.h"
#include "Metaheuristic.h"
#include <algorithm>
#include <iostream>
#include "const.h"

Metaheuristic::Metaheuristic() {}

Metaheuristic::~Metaheuristic() {}

//This function will calculate the gain in dollars with the capacitor of 300kVAr installation in the bus
float Metaheuristic::objectiveFunction(float lossGainLight, float lossGainMedium, float lossGainHeavy) {
	float profit = energy::pricekWh * ((lossGainLight * general::lightLoadHours) + (lossGainMedium * general::mediumLoadHours) + (lossGainHeavy * general::heavyLoadHours)) -  (capacitor::price300kVAr + capacitor::price300kVArInstalation);
	return profit;
}

//This function will calculate the gain in dollars with all the capacitors in the grid
float Metaheuristic::objectiveFunctionFinal(float lossGainLight, float lossGainMedium, float lossGainHeavy, std::vector<Solution*> &solutions) {
	int numCap = 0;
	float profit = energy::pricekWh * ((lossGainLight * general::lightLoadHours) + (lossGainMedium * general::mediumLoadHours) + (lossGainHeavy * general::heavyLoadHours)) - (solutions.size() * (capacitor::price300kVArInstalation + capacitor::price300kVAr));
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
	pFlow.level = loadLevel::light;
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
	lossGainLight = (lossLight - lossLightCap) / 1000;
	lossGainMedium = (lossMedium - lossMediumCap) / 1000;
	lossGainHeavy = (lossHeavy - lossHeavyCap) / 1000;

	gain = this->objectiveFunction(lossGainLight, lossGainMedium, lossGainHeavy);

	return gain;
}

float Metaheuristic::calculateGainFinal(Circuit* pCircReference, Circuit* pCirc, std::vector<Solution*> &solutions) {
	float lossGainLight{ 0 }, lossGainMedium{ 0 }, lossGainHeavy{ 0 };
	float lossLight{ 0 }, lossMedium{ 0 }, lossHeavy{ 0 };
	float lossLightRef{ 0 }, lossMediumRef{ 0 }, lossHeavyRef{ 0 };
	float gain{ 0 };

	Flow pFlow(pCirc, general::voltageReference, general::lossTolerance);
	Flow pFlowRef(pCircReference, general::voltageReference, general::lossTolerance);

	//Flow for circuit
	pFlow.level = loadLevel::light;
	pFlow.execute();
	lossLight = pFlow.loss;

	pFlow.level = loadLevel::medium;
	pFlow.execute();
	lossMedium = pFlow.loss;

	pFlow.level = loadLevel::heavy;
	pFlow.execute();
	lossHeavy = pFlow.loss;

	//Running flow for circuit reference
	pFlowRef.level = loadLevel::light;
	pFlowRef.execute();
	lossLightRef = pFlowRef.loss;

	pFlowRef.level = loadLevel::medium;
	pFlowRef.execute();
	lossMediumRef = pFlowRef.loss;

	pFlowRef.level = loadLevel::heavy;
	pFlowRef.execute();
	lossHeavyRef = pFlowRef.loss;


	//Change from W to kW
	lossGainLight = (lossLightRef - lossLight) / 1000;
	lossGainMedium = (lossMediumRef - lossMedium) / 1000;
	lossGainHeavy = (lossHeavyRef - lossHeavy) / 1000;

	gain = Metaheuristic::objectiveFunctionFinal(lossGainLight, lossGainMedium, lossGainHeavy, solutions);

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
	pFlow.level = loadLevel::light;
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

float Metaheuristic::calculateLosskW(Circuit *pCirc, int level) {

	float loss{ 0 };

	Flow pFlow(pCirc, general::voltageReference, general::lossTolerance);

	switch (level) {
		case 0:
			//Flow for medium load with no capacitor
			pFlow.level = loadLevel::light;
			pFlow.execute();
			loss = pFlow.loss;

			//Change from W to kW
			loss = loss / 1000;
			break;
		case 1:
			//Flow for medium load with no capacitor
			pFlow.level = loadLevel::medium;
			pFlow.execute();
			loss = pFlow.loss;

			//Change from W to kW
			loss = loss / 1000;
			break;
		case 2:
			//Flow for medium load with no capacitor
			pFlow.level = loadLevel::heavy;
			pFlow.execute();
			loss = pFlow.loss;

			//Change from W to kW
			loss = loss / 1000;
			break;
		default:
			break;
	}
	
	return loss;
}

float Metaheuristic::calculateLosskWh(Circuit *pCirc) {

	float lossLight{ 0 }, lossMedium{ 0 }, lossHeavy{ 0 };
	float loss{ 0 };

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

	//Change from W to kW
	lossLight = lossLight / 1000;
	lossMedium = lossMedium / 1000;
	lossHeavy = lossHeavy/ 1000;
	
	loss = (lossLight * general::lightLoadHours) + (lossMedium * general::mediumLoadHours) + (lossHeavy * general::heavyLoadHours);

	return loss;
}
int Metaheuristic::numberOfCap(std::vector<Solution*> &solutions) {
	auto end = solutions.end();
	int capNum = 0;
	for (auto it = solutions.begin(); it != end; ++it) {
		capNum += (*it)->pBus->numberOfCap;
	}
	return capNum;
}
void Metaheuristic::log(std::vector<Solution*> &solutions, Circuit* pCircReference, Circuit* pCirc) {
	auto end = solutions.end();
	int capNum = 0;
	for (auto it = solutions.begin(); it != end; ++it) {
		end = std::remove(it + 1, end, *it);
	}
	end = end - 2;
	for (auto it = solutions.begin(); it != end; it++) {
		if((*it)->pBus->numberOfCap != 0)std::cout <<"Barra "<<(*it)->pBus->code<<": "<<(*it)->pBus->numberOfCap<<" capacitores"<<std::endl;
		capNum += (*it)->pBus->numberOfCap;
	}
	std::cout << std::endl;
	std::cout << capNum << " capacitores alocados " << std::endl;

	std::cout << "Perdas antes em kW para o patamar leve de carga: " << Metaheuristic::calculateLosskW(pCircReference, 0) << " kW" << std::endl;
	std::cout << "Perdas depois em kW para o patamar leve de carga: " << Metaheuristic::calculateLosskW(pCirc, 0) << " kW" << std::endl;
	std::cout << std::endl;

	std::cout << "Perdas antes em kW para o patamar medio de carga: " << Metaheuristic::calculateLosskW(pCircReference, 1) << " kW" << std::endl;
	std::cout << "Perdas depois em kW para o patamar medio de carga: " << Metaheuristic::calculateLosskW(pCirc, 1) << " kW" << std::endl;
	std::cout << std::endl;

	std::cout << "Perdas antes em kW para o patamar pesado de carga: " << Metaheuristic::calculateLosskW(pCircReference, 2) << " kW" << std::endl;
	std::cout << "Perdas depois em kW para o patamar pesado de carga: " << Metaheuristic::calculateLosskW(pCirc, 2) << " kW" << std::endl;
	std::cout << std::endl;

	std::cout <<"Perdas antes em kWh para um ano: " << Metaheuristic::calculateLosskWh(pCircReference) <<" kWh" << std::endl;
	std::cout <<"Perdas depois em kWh para um ano: " << Metaheuristic::calculateLosskWh(pCirc) << " kWh" << std::endl;
	std::cout << std::endl;

	
	std::cout << "Ganho de " << Metaheuristic::calculateGainFinal(pCircReference, pCirc, solutions) << " dolares" << std::endl;
	std::cout << std::endl;
}