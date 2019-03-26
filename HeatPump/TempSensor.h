#pragma once
#include "Sensor.h"
#include "TemperatureDriver.h"
#include <DallasTemperature.h>

#define TEMPERATURE_FAILED -999

class TempSensor :
	public Sensor
{
public:
	TempSensor(String label, int pin, float lowerRange, float upperRange);
	
	~TempSensor();
	bool loop();
	bool checkDataReady();
	DeviceAddress DevAddress;
	bool DevAvailable = false;
	TemperatureDriver* Driver;
	void begin();
	byte* GetMac() { return DevAddress; };
private:

};

