#pragma once
#include <string>
#include <complex>
#include "Equipment.h"

//LO: Load, GR: Generator, CP: Capacitor, RE:Reactor 
enum TYPEOFSHUNTELEMENT { LO, GR, CP, RE };

//RE: Residential, COM: Comercial, IND: Industry, SP: Public Service, HOS: Hospital
enum TYPEOFLOAD { RES, COM, IND, SP, HOS };

class Bus : public Equipment
{
public:
	Bus();
	~Bus();
	Bus(Bus &other);
	Bus *pBus1, *pBus2;
	int                 code, numberOfClients, numberOfCap;
	TYPEOFSHUNTELEMENT  shuntType;
	TYPEOFSHUNTELEMENT  TypeOfShuntElement(std::string);
	TYPEOFLOAD          loadType;
	TYPEOFLOAD          TypeOfLoad(std::string);
	//Nominal Power: Pnom, Complex Voltage: V,
	//Complex Current: I, superfice available on the bus for the load flow: loadArea,
	std::string         name;
	std::complex<float> V[3], I[3], power[3];
	float               loadArea, Pnom;
	Bus& operator=(const Bus &other);
	bool operator==(const Bus &other) const;
	void setBus1(Bus *pBus);
	void setBus2(Bus *pBus);
};