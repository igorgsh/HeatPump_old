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
const byte PINS = 13; // number of sensors
const byte CASES = 1;
byte rowKeyPadPins[ROWS] = { 14,15,16,17 };
byte colKeyPadPins[COLS] = { 18, 19, 20, 21 };
const byte pinMap[PINS] = { 22,23,24,25,26,27,28,29,30,31,32,33,8/*,38,40,42,44*/};

Simulator* sim;

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

	//Set N0 regular mode
	res = sim->SetValueGen(0, 22, 12.0);
	if (!res) Debug("T1 Error");
	res = sim->SetValueGen(0, 23, 6.0);
	if (!res) Debug("T2 Error");
	
	res = sim->SetValueGen(0, 24, 30.0);
	if (!res) Debug("T3 Error");
	res = sim->SetValueGen(0, 25, 10.0);
	if (!res) Debug("T4 Error");
	
	res = sim->SetValueGen(0, 26, 50.0);
	if (!res) Debug("T5 Error");
	res = sim->SetValueGen(0, 27, 30.0);
	if (!res) Debug("T6 Error");

	res = sim->SetValueGen(0, 28, 32.0);
	if (!res) Debug("T7 Error");

	res = sim->SetValueGen(0, 29, 40);
	if (!res) Debug("T8 Error");

	res = sim->SetValueGen(0, 30, 80.0);
	if (!res) Debug("T9 Error");

	res = sim->SetValueGen(0, 31, 28.0);
	if (!res) Debug("T10 Error");
	res = sim->SetValueGen(0, 32, 22.0);
	if (!res) Debug("T11 Error");
	res = sim->SetValueGen(0, 33, 17.0);
	if (!res) Debug("T12 Error");

	res = sim->SetValueGen(0, 8, 1.0);
	if (!res) Debug("Contactor Error");

	sim->SetValueGenNumber(0);
}


#endif
#endif

