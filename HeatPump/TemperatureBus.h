// TempSensorMultiple.h
#pragma once


#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "OneWire.h"
#include "DallasTemperature.h"
#include "TemperatureBus.h"
#include "TemperatureDriver.h"

#define DT_DEFAULT_RESOLUTION 10


class TemperatureBus : public TemperatureDriver
{
  public:
	 bool begin();
	 TemperatureBus(int pin);
	 ~TemperatureBus();
	 void RequestTemperature() { dt->requestTemperatures(); }
	 float GetTemperature(DeviceAddress addr) { return dt->getTempC(addr); }
  private:
	OneWire* bus;
	DallasTemperature* dt;
	TempSensor* sensors;
	int numberOfSensors = 0;
	DeviceAddress* knownDevs;
	bool* knownDevAvail;
	int numberUnknownSensors = 0;
	bool compareAddresses(DeviceAddress addr1, DeviceAddress addr2);
};

