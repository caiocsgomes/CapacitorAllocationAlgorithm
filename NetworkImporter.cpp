#include "stdafx.h"
#include <string>
#include "NetworkImporter.h"
#include "Bus.h"
#include "Branch.h"
#include "Equipment.h"
#include "Circuit.h"

Circuit* NetworkImporter::NetworkFromTxt(std::string filename) {
	std::ifstream inputFile;
	std::string line;
	Bus *pBus;
	Branch *pBranch;
	Circuit *pCirc = new Circuit();
	inputFile.open(filename);

	if (!inputFile.is_open()) return nullptr;
	
	std::getline(inputFile, line, ',');
	
	do  {
		switch (this->TypeOfElement(line)) {
			case BUS:

				//Extraction of the end of line character after reading the word
				std::getline(inputFile, line, '\n');

				std::getline(inputFile, line, ',');
				do  {
					pBus = new Bus();

					pBus->code = stoi(line);
					
					std::getline(inputFile, line, ',');
					pBus->power[0].real(stof(line));
					
					std::getline(inputFile, line, '\n');
					pBus->power[0].imag(stof(line));
					
					pCirc->busVector.push_back(pBus);			

					//Extraction of the first element to check if it is an electrical type of if
					//it is still a property of Bus
					std::getline(inputFile, line, ',');
				} while (!isKeyWord(line)); break;
			case BRANCH:

				//Extraction of the end of line character after reading the word
				std::getline(inputFile, line, '\n');

				std::getline(inputFile, line, ',');
				do {
					pBranch = new Branch();

					Bus *pBus1 = pCirc->getBusById(stoi(line));
					pBranch->setBus1(pBus1);

					std::getline(inputFile, line, ',');
					Bus *pBus2 = pCirc->getBusById(stoi(line));
					pBranch->setBus2(pBus2);

					pBus1->setBus2(pBus2);
					pBus2->setBus1(pBus1);
					
					std::getline(inputFile, line, ',');
					pBranch->Z.real(stof(line));
					
					std::getline(inputFile, line, '\n');
					pBranch->Z.imag(stof(line));

					pCirc->branchVector.push_back(pBranch);
					
					//Extraction of the first element to check if it is an electrical type of if
					//it is still a property of Branch
					std::getline(inputFile, line, ',');
				} while (!isKeyWord(line)); break;
			default: break;
		} 
	} while (line != "FIM");

	if (pCirc) return pCirc;
	else return nullptr;
}

TYPEOFELECTRICALELEMENT NetworkImporter::TypeOfElement(std::string name) {
	if		(name == "LINHA") return BRANCH; 
	else if (name == "BARRA") return BUS;
}

bool NetworkImporter::isKeyWord(std::string keyword) {
	if ((keyword == "LINHA") || (keyword == "BARRA") || (keyword == "FIM")) return true;
	return false;
}
