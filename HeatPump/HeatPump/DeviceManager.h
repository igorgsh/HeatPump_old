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

	TempSensor tempSensors[NUMBER_OF_TEMP] = { TempSensor("D1",30,21.0,23.0, 5),
		TempSensor("D2",31,22.0,25.0,  6) };

	int getNumberTemp() { return NUMBER_OF_TEMP; }
	int getNumberCont() { return NUMBER_OF_CONTACTOR; }

};

