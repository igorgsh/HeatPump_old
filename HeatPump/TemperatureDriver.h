#pragma once
#include <DallasTemperature.h>

class TemperatureDriver
{
public:
	TemperatureDriver();
	~TemperatureDriver();
	virtual void RequestTemperature() =0;
	virtual float GetTemperature(DeviceAddress addr) = 0;
};

