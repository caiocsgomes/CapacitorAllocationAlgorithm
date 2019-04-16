#include "stdafx.h"
#include "Circuit.h"

Circuit::Circuit() {};

Circuit::~Circuit() {
	for (Bus *pBus : this->busVector) delete pBus;
	for (Branch *pBranch : this->branchVector) delete pBranch;
};

//The buses in the txt come with the id in a linear order (1, 2, 3..., n)
//So we just need to return the bus in the position (id - 1), considering the vector has a zero based index 
Bus* Circuit::getBusById(int id) {
	if (this->busVector[id - 1]) return this->busVector[id - 1];
	return nullptr;
}