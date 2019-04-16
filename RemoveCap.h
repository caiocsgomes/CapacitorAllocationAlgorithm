#pragma once
#include "Metaheuristic.h"

class Circuit;

class RemoveCap : public Metaheuristic
{
public:
	RemoveCap();
	~RemoveCap();
	bool execute(std::vector<Solution*> &solutions, Circuit *pCirc);
};

