#include "stdafx.h"
#include <string>
#include <map>
#include <ostream>
#include "Bus.h"

std::map<std::string, TYPEOFLOAD> mapTypeOfLoad{
	{ "0", RES },
	{ "1", COM },
	{ "2", IND },
	{ "3", SP },
	{ "4", HOS }
};

std::map<std::string, TYPEOFSHUNTELEMENT> mapTypeOfShuntElement{
	{ " ", LO },
	{ "", LO },
	{ "GR", GR },
	{ "CP", CP },
	{ "RE", RE }
};

//The f after 0.0 in the power initialization is to avoid the compiler giving narrowing conversion, error
//even though this is not a narrowing conversion problem, its a compiler bug from Visual Studio
//See 8.5.4 C++14 Norm
Bus::Bus():
	pBus1(nullptr), pBus2(nullptr), code(-1), numberOfClients(0), numberOfCap(0), shuntType(LO), loadType(RES), name(""), V{ (0.0f, 0.0f), (0.0f, 0.0f),
	(0.0f ,0.0f) }, I{ (0.0f, 0.0f), (0.0f, 0.0f), (0.0f ,0.0f) }, power{ (0.0f, 0.0f), (0.0f, 0.0f), (0.0f ,0.0f) }, loadArea(0), Pnom(0) { };

Bus::~Bus() {};

Bus::Bus(Bus &other) {
	this->code = -other.code;
	this->I[0] = other.I[0];
	this->V[0] = other.V[0];
	this->loadArea = other.loadArea;
	this->loadType = other.loadType;
	this->power[0] = other.power[0];
	this->shuntType = other.shuntType;
	this->stateType = other.stateType;
}

Bus& Bus::operator=(const Bus &other) {
	if (this == &other) return *this;
	this->I[0] = other.I[0];
	this->V[0] = other.V[0];
	this->loadArea = other.loadArea;
	this->loadType = other.loadType;
	this->power[0] = other.power[0];
	this->shuntType = other.shuntType;
	this->stateType = other.stateType;
	return *this;
};

bool Bus::operator==(const Bus &other) const {
	return (code == other.code);
};

//Useful for debug
std::ostream &operator<<(std::ostream &out, const Bus &bus) {
	out << bus.power[0];
	return out;
};

TYPEOFSHUNTELEMENT Bus::TypeOfShuntElement(std::string input) {
	return mapTypeOfShuntElement[input];
}

TYPEOFLOAD Bus::TypeOfLoad(std::string input) {
	return mapTypeOfLoad[input];
};

void Bus::setBus1(Bus *pBus) {
	this->pBus1 = pBus;
};

void Bus::setBus2(Bus *pBus) {
	this->pBus2 = pBus;
};