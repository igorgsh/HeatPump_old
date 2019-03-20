#pragma once
#include "Sensor.h"

#include <DallasTemperature.h>

class TempSensor :
	public Sensor
{
public:
	TempSensor();
	
	~TempSensor();
	DeviceAddress addr;
};

