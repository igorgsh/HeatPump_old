#pragma once
#include <DallasTemperature.h>

class TemperatureDriver
{
public:
	TemperatureDriver();
	virtual ~TemperatureDriver();
	virtual void RequestTemperature() =0;
	virtual float GetTemperature(DeviceAddress addr) = 0;
	virtual void loop() = 0;
};

