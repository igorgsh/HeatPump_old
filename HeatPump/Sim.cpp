// 
// 
// 

#include "Sim.h"


void initSim() {
	sim = new Simulator(CASES0, PINS0, pinMap0, rowKeyPadPins0, colKeyPadPins0);
	bool res;

	//Set N0 regular mode. Compressor should start!
	res = sim->SetPinValue(0, 22, 12.0);
	if (!res) Debug("0.T1 Error");
	res = sim->SetPinValue(0, 23, 6.0);
	if (!res) Debug("0.T2 Error");

	res = sim->SetPinValue(0, 24, 30.0);
	if (!res) Debug("0.T3 Error");
	res = sim->SetPinValue(0, 25, 10.0);
	if (!res) Debug("0.T4 Error");

	res = sim->SetPinValue(0, 26, 50.0);
	if (!res) Debug("0.T5 Error");
	res = sim->SetPinValue(0, 27, 30.0);
	if (!res) Debug("0.T6 Error");

	res = sim->SetPinValue(0, 28, 32.0);
	if (!res) Debug("0.T7 Error");

	res = sim->SetPinValue(0, 29, 40);
	if (!res) Debug("0.T8 Error");

	res = sim->SetPinValue(0, 30, 80.0);
	if (!res) Debug("0.T9 Error");

	res = sim->SetPinValue(0, 31, 28.0);
	if (!res) Debug("0.T10 Error");
	res = sim->SetPinValue(0, 32, 22.0);
	if (!res) Debug("0.T11 Error");
	res = sim->SetPinValue(0, 33, 17.0);
	if (!res) Debug("0.T12 Error");

	res = sim->SetPinValue(0, 99, 28);
	if (!res) Debug("0.Desired Temp Error");


	res = sim->SetPinValue(0, 8, 1.0);
	if (!res) Debug("0.Contactor Error");

	//Set N1 regular mode. Compressor should stop!
	sim->SetCaseValues(1, 0);
	res = sim->SetPinValue(1, 32, 34.0);
	if (!res) Debug("1.T10 Error");

	//Set N2 regular mode. Compressor should not start due low desired temp!
	sim->SetCaseValues(2, 0);
	res = sim->SetPinValue(2, 99, 21);
	if (!res) Debug("2.Desired Temp Error");
	res = sim->SetPinValue(2, 31, 28.0);
	if (!res) Debug("2.T10 Error");

	//Set N3 emergency mode. Compressor should start but some sensor is failed!
	sim->SetCaseValues(3, 0);
//	res = sim->SetPinValue(3, 8, 0.0);
//	if (!res) Debug("3.Contactor Error");
	res = sim->SetPinValue(3, 99, 30);
	if (!res) Debug("3.Desired Temp Error");
	res = sim->SetPinValue(3, 31, 28.0);
	if (!res) Debug("3.T10 Error");



	sim->SetCaseNumber(0);
}
