#include "stdafx.h"
#include <complex>
#include "Flow.h"
#include "Bus.h"
#include "Branch.h"
#include "Circuit.h"
int Flow::numFlow = 0;

Flow::Flow(Circuit* circ, std::complex<float> voltage, float referenceTolerance)
	: pCirc(circ), tolerance(referenceTolerance), loss(0), level(1), voltageReference(voltage),
	  oldLosses(0), newLosses(0), numberOfIterations(0) {};
Flow::~Flow() {};

//This method will initialize the voltage in all buses with the reference voltage (last bus) 
//And the losses for the circuit
void Flow::startVoltages() {
	for (Bus* pBus : this->pCirc->busVector) {
		pBus->V[0] = this->voltageReference;
	}
	this->oldLosses = 0;
	this->newLosses = 0;
};

void Flow::refreshLosses() {
	this->oldLosses      = this->newLosses;
};

/*Formulas to calculate Real and Imaginary parts of the current:
	Real Current      = (Real Power * Real Voltage + Imag Power * Imag Current)/((Real Voltage)^2 + (Imag Voltage)^2)
	Imaginary Current = (Real Power * Imag Voltage - Imag Power * Real Current)/((Real Voltage)^2 + (Imag Voltage)^2)
	
	level refers to the use of the electrical grid, it can be heavy (a lot o people using it), like at night when you get home and use
	a lot of electrical devices, medium, like in the morning, when you use electrical devices but not a lot, and light, like during the
	afternoon when you are at work and nobody is at home. Level is multiplied to power (consumption).
*/
void Flow::calculateCurrentBus() {
	for (int index = 0; index < this->pCirc->busVector.size() - 1; index++) {
		pCirc->busVector[index]->I[0].real((pCirc->busVector[index]->power[0].real() * level * pCirc->busVector[index]->V[0].real() + pCirc->busVector[index]->power[0].imag() * level * pCirc->busVector[index]->V[0].imag()) / (pow(pCirc->busVector[index]->V[0].real(), 2) + pow(pCirc->busVector[index]->V[0].imag(), 2)));
		pCirc->busVector[index]->I[0].imag((pCirc->busVector[index]->power[0].real() * level * pCirc->busVector[index]->V[0].imag() - pCirc->busVector[index]->power[0].imag() * level * pCirc->busVector[index]->V[0].real()) / (pow(pCirc->busVector[index]->V[0].real(), 2) + pow(pCirc->busVector[index]->V[0].imag(), 2)));
	}
};

/*
	The current in a Branch is the current in the downstream Bus plus the current in the downstream Branch (Kirchhoff's Current Law)
*/
void Flow::calculateCurrentBranch() {
	for (int index = pCirc->branchVector.size() - 1; index >= 0; index--) {
		pCirc->branchVector[index]->pbus1->I[0] += pCirc->branchVector[index]->pbus2->I[0];
	}
};

/*
	The Power Loss in circuit is the sum of the loss in all branches of the circuit
*/
void Flow::calculatePowerLoss() {
	this->newLosses = 0;
	for (Branch* pBranch : this->pCirc->branchVector) {
		this->realLoss = pBranch->Z.real() * (pow(pBranch->pbus2->I[0].real(), 2));
		this->imaginaryLoss = pBranch->Z.real() * (pow(pBranch->pbus2->I[0].imag(), 2));
		this->newLosses += realLoss + imaginaryLoss;
	}
};

float Flow::calculateLossDifference() {
	return abs(this->newLosses - this->oldLosses);
};

void Flow::calculateVoltageBus() {
	for (int index = 0; index < this->pCirc->branchVector.size(); index++) {
		this->pCirc->branchVector[index]->pbus2->V[0] = this->pCirc->branchVector[index]->pbus1->V[0] - (this->pCirc->branchVector[index]->pbus2->I[0] * this->pCirc->branchVector[index]->Z)/(std::complex<float>(1000,0));
	}
};

void Flow::execute() {
	this->startVoltages();
	numberOfIterations = 0;
	do {
		this->refreshLosses();
		this->calculateCurrentBus();
		this->calculateCurrentBranch();
		this->calculateVoltageBus();
		this->calculatePowerLoss();
		numberOfIterations++;
	} while (this->calculateLossDifference() > this->tolerance && numberOfIterations < 300);
	if (numberOfIterations >= 300) throw std::string("Convergence problem in flow");
	this->loss = this->newLosses;
	Flow::numFlow++;
}