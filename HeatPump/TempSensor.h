#pragma once
#include "Sensor.h"
#include "TemperatureDriver.h"
#include <DallasTemperature.h>

class TempSensor :
	public Sensor
{
public:
	TempSensor(String label, TemperatureDriver* drv, float lowerRange, float upperRange);
	
	~TempSensor();
	bool loop();
	bool checkDataReady();
	DeviceAddress DevAddress;
	bool DevAvailable = false;
private:

	TemperatureDriver* driver;
};

