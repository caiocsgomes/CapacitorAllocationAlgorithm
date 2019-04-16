#pragma once
#include "Metaheuristic.h"


//This class will add solutions to the bars that contain already solutions
class AddCapBusHasCap :
	public Metaheuristic
{
public:
	AddCapBusHasCap();
	~AddCapBusHasCap();
	bool execute(std::vector<Solution*> &solutions, Circuit *pCirc);
};

