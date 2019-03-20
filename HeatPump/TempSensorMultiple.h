// TempSensorMultiple.h
#pragma once

#ifndef _TEMPSENSORMULTIPLE_h
#define _TEMPSENSORMULTIPLE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "OneWire.h"
#include "DallasTemperature.h"
#include "TempSensor.h"

class TempSensorMultiple : public Sensor
{
  public:
	 bool begin();
	 TempSensorMultiple(String label, int pin);
	 ~TempSensorMultiple();
  private:
	OneWire* bus;
	DallasTemperature* dt;
	TempSensor* sensors;
	int numberOfSensors = 0;
};

#endif

