#pragma once
#include <complex>
#include "Equipment.h"

class Bus;

enum TYPEOFBRANCHELEMENT{ LI, CF, CS, DJ, RL, SL, RT, TR };

class Branch : public Equipment
{
public:
	Branch();
	Branch(const Branch &other);
	~Branch();
	Branch& operator=(const Branch &other);
	Bus *pbus1;
	Bus *pbus2;
	TYPEOFBRANCHELEMENT lineElement;
	std::complex<float> Z;
	std::complex<float> I[3];
	int code;
	float loadArea, Inom, Iadm;
	bool operator==(const Branch &other);
	bool operator!=(const Branch &other);
	void setBus1(Bus *bus);
	void setBus2(Bus *bus);
	void setTypeElement(std::string);
};