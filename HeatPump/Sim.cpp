// 
// 
// 

#include "Sim.h"
#include "Loger.h"

void initSim() {
	sim = new Simulator(CASES0, PINS0, pinMap0, rowKeyPadPins0, colKeyPadPins0);
	
	//Set N0 regular mode. Compressor should start!
	sim->SetPinValue(0, 61, 12.0);
	sim->SetPinValue(0, 62, 6.0);
	sim->SetPinValue(0, 63, 30.0);
	sim->SetPinValue(0, 64, 10.0);
	sim->SetPinValue(0, 65, 50.0);
	sim->SetPinValue(0, 66, 30.0);
	sim->SetPinValue(0, 67, 32.0);
	//sim->SetPinValue(0, 29, 40);
	//sim->SetPinValue(0, 30, 80.0);
	//sim->SetPinValue(0, 31, 28.0);
	//sim->SetPinValue(0, 32, 22.0);
	//sim->SetPinValue(0, 33, 17.0);

	sim->SetPinValue(0, 99, 28);
	
	sim->SetPinValue(0, 8, 1.0);

	//Set N1 regular mode. Compressor should stop!
	sim->SetCaseValues(1, 0);
	sim->SetPinValue(1, 64, 34.0);

	//Set N2 regular mode. Compressor should not start due low desired temp!
	sim->SetCaseValues(2, 0);
	sim->SetPinValue(2, 99, 21);
	//sim->SetPinValue(2, 31, 28.0);

	//Set N3 emergency mode. Compressor should start but some sensor is failed!
	sim->SetCaseValues(3, 0);
//	res = sim->SetPinValue(3, 8, 0.0);
//	if (!res) Debug("3.Contactor Error");
//	res = sim->SetPinValue(3, 99, 30);
//	if (!res) Debug("3.Desired Temp Error");
//	res = sim->SetPinValue(3, 31, 28.0);
//	if (!res) Debug("3.T10 Error");



	sim->SetCaseNumber(0);
}
