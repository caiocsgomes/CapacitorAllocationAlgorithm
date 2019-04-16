#pragma once
#include <string>

//Set which phases the equipment is connected
enum PHASES { A, B, C, AB, AC, BC, ABC };

//desligado: off, ligado: on
enum STATE { desligado, ligado };

class Equipment
{
public:
	Equipment();
	~Equipment();
	PHASES phasesType;
	PHASES TypeOfPhases(std::string);
	STATE stateType;
	STATE TypeOfState(std::string);
};