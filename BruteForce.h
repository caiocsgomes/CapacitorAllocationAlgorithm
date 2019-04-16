#pragma once
#include "Metaheuristic.h"

class Circuit;

class BruteForce : public Metaheuristic
{
public:
	BruteForce();
	~BruteForce();
	std::vector<Solution*> execute(Circuit *pCirc);
};

