#pragma once

#include "TempSensor.h"
#define NUMBER_OF_TEMP	2
#define NUMBER_OF_CONTACTOR	2

class DeviceManager
{
public:
	DeviceManager();
	~DeviceManager();

	void begin();

	TempSensor tempSensors[NUMBER_OF_TEMP] = { TempSensor("T1",22,new float[NUMBER_OF_ACTIONPOINTS]{21.0,22.0,24.0,25.0}, 5),
		TempSensor("T2",23,new float[NUMBER_OF_ACTIONPOINTS]{ 21.0,22.0,24.0,25.0 },5) };

	int getNumberTemp() { return NUMBER_OF_TEMP; }
	int getNumberCont() { return NUMBER_OF_CONTACTOR; }
	void loop1(unsigned long counter);
	void loop5(unsigned long counter);
	void loop10(unsigned long counter);
};

