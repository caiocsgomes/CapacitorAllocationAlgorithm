#pragma once
#include "Metaheuristic.h"

class Circuit;

class AddCapBusWithNoCap :
	public Metaheuristic
{
public:
	AddCapBusWithNoCap();
	~AddCapBusWithNoCap();
	bool execute(std::vector<Solution*> &currentSolution, std::vector<Solution*> initialSolution, Circuit *pCirc);
};

