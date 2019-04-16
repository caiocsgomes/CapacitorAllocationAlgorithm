#pragma once
#include <vector>
#include "Metaheuristic.h"

class Circuit;

class ReconfigureSolutions : public Metaheuristic
{
public:
	ReconfigureSolutions();
	~ReconfigureSolutions();
	std::vector<Solution*> execute(std::vector<Solution*> solutions, Circuit *pCirc);
};

