#pragma once
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>

class Circuit;

enum TYPEOFELECTRICALELEMENT {BUS, BRANCH};

class NetworkImporter
{
public:
	Circuit* NetworkFromTxt(std::string filename);
private:
	bool isKeyWord(std::string keyword);
	TYPEOFELECTRICALELEMENT TypeOfElement(std::string name);
};

