// Sim.h

#ifdef _SIMULATOR_
#ifndef _SIM_h
#define _SIM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Simulator.h"

const byte ROWS = 4; // 4 строки
const byte COLS = 4; // 4 столбца
const byte PINS = 14; // number of sensors
const byte CASES = 4;
byte rowKeyPadPins[ROWS] = { 14,15,16,17 };
byte colKeyPadPins[COLS] = { 18, 19, 20, 21 };
const byte pinMap[PINS] = { 22,23,24,25,26,27,28,29,30,31,32,33,8,99/*,38,40,42,44*/};

extern Simulator* sim;

void initSim() {
	sim = new Simulator(CASES, PINS, pinMap, rowKeyPadPins, colKeyPadPins);
	bool res;

/*
	TempSensor("GeoI", 22, new float[NUMBER_OF_ACTIONPOINTS] {-20.0, -15.0, 40.0, 50.0}, 5),	//T1
		TempSensor("GeoO", 23, new float[NUMBER_OF_ACTIONPOINTS] {-20.0, -15.0, 40.0, 50.0}, 5),	//T2
		TempSensor("EvoI", 24, new float[NUMBER_OF_ACTIONPOINTS] {-30.0, -15.0, 40.0, 50.0}, 5),	//T3 = T6
		TempSensor("EvoO", 25, new float[NUMBER_OF_ACTIONPOINTS] {-30.0, -15.0, 40.0, 50.0}, 5),	//T4
		TempSensor("CondI", 26, new float[NUMBER_OF_ACTIONPOINTS] {-30.0, -15.0, 140.0, 150.0}, 5),	//T5
		TempSensor("CondO", 27, new float[NUMBER_OF_ACTIONPOINTS] {-30.0, -15.0, 40.0, 50.0}, 5),	//T6 = T3
		TempSensor("HpO", 28, new float[NUMBER_OF_ACTIONPOINTS] {-30.0, -15.0, 100.0, 110.0}, 5),	//T7
		TempSensor("HpI", 29, new float[NUMBER_OF_ACTIONPOINTS] {-30.0, -15.0, 100.0, 110.0}, 5),	//T8
		TempSensor("Comp", 30, new float[NUMBER_OF_ACTIONPOINTS] { -30.0, -15.0, 117.0, 120.0 }, 5),//T9
		TempSensor("Unkn", 31, new float[NUMBER_OF_ACTIONPOINTS] { -50.0, -50.0, 50.0, 50.0 }, 5),	//T10
		TempSensor("Room", 32, new float[NUMBER_OF_ACTIONPOINTS] { -50.0, -50.0, 50.0, 50.0 }, 5),	//T11
		TempSensor("Street", 33, new float[NUMBER_OF_ACTIONPOINTS] { -50.0, -50.0, 50.0, 50.0 }, 5)	//T12
		
		Contactor contacts[NUMBER_OF_CONTACTOR] = {Contactor("Flow", 8, true, 5)};

*/																									//

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
	res = sim->SetPinValue(0, 99, 22);
	if (!res) Debug("2.Desired Temp Error");

	//Set N3 emergency mode. Compressor should start but some sensor is failed!
	sim->SetCaseValues(3, 0);
	res = sim->SetPinValue(3, 8, 0.0);
	if (!res) Debug("3.Contactor Error");



	sim->SetCaseNumber(0);
}


#endif
#endif

