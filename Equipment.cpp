#include "stdafx.h"
#include "Equipment.h"
#include <map>


Equipment::Equipment()
	: phasesType(ABC), stateType(ligado) {};

Equipment::~Equipment() {};

std::map<std::string, PHASES> mapOfPhases{
	{ "A", A },
	{ "B", B },
	{ "C", C },
	{ "AB", AB },
	{ "AC", AC },
	{ "BC", BC },
	{ "ABC", ABC }
};

PHASES Equipment::TypeOfPhases(std::string input) {
	return mapOfPhases[input];
};

std::map<std::string, STATE> mapOfState{
	{ "0", desligado },
	{ "1", ligado }
};

STATE Equipment::TypeOfState(std::string input) {
	return mapOfState[input];
};