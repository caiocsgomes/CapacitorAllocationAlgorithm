#pragma once
#include "Metaheuristic.h"
class AddCapNeighbour :
	public Metaheuristic
{
public:
	AddCapNeighbour();
	~AddCapNeighbour();
	bool execute(std::vector<Solution*> &solutions, Circuit *pCirc);
};

