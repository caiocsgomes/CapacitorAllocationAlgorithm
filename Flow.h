#pragma once
#include <complex>

class Circuit;

class Flow
{
public:
	Flow(Circuit* circ, std::complex<float> voltage, float referenceTolerance);
	~Flow();	
	Circuit* pCirc;
	float tolerance, loss, level;
	void  execute(void);
	static int numFlow;
private:
	std::complex<float> voltageReference;
	float oldLosses, newLosses, imaginaryLoss, realLoss;
	int   numberOfIterations;
	void  startVoltages(void);
	void  refreshLosses(void);
	void  calculateCurrentBus(void);
	void  calculateCurrentBranch(void);
	float calculateLossDifference(void);
	void  calculatePowerLoss(void);
	void  calculateVoltageBus(void);
};

