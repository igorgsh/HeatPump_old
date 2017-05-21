#pragma once

#include "TempSensor.h"
#include "Pump.h"
#include "Compressor.h"

#define NUMBER_OF_TEMP	2
#define NUMBER_OF_CONTACTOR	2
#define NUMBER_OF_PUMP	3

class DeviceManager
{
public:
	DeviceManager();
	~DeviceManager();

	void begin();

	TempSensor tempSensors[NUMBER_OF_TEMP] = {	TempSensor("T1",22,new float[NUMBER_OF_ACTIONPOINTS] {21.0,22.0,24.0,25.0}, 5),
												TempSensor("T2",23,new float[NUMBER_OF_ACTIONPOINTS] { 21.0,22.0,24.0,25.0 },5) };

	Pump pumps[NUMBER_OF_PUMP] = {	Pump("P1", 24,LOW,30,30),
									Pump("P2", 25,LOW,30,30),
									Pump("P3", 26,LOW,30,30),
								};
	Compressor compressor = Compressor("C1", 27, LOW, 30, 30);

	Pump* pumpGeo = &pumps[0];
	Pump* pumpContour1 = &pumps[1];
	Pump* pumpContour2 = &pumps[2];

	TempSensor* tGeo = &tempSensors[0];
	TempSensor* t1 = &tempSensors[1];


	int getNumberTemp() { return NUMBER_OF_TEMP; }
	int getNumberCont() { return NUMBER_OF_CONTACTOR; }
	int getNumberPump() { return NUMBER_OF_PUMP; }
	void loop1(unsigned long counter);
	void loop5(unsigned long counter);
	void loop10(unsigned long counter);
};

