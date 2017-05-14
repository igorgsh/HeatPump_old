#pragma once
#include "Sensor.h"
#include "DT.h"
#include "OneWire.h"
#define DEFAULT_RESOLUTION 10


class TempSensor :
	public Sensor
{
public:
	~TempSensor();
	TempSensor(String label, int pin, float actionLow, float actionHigh, int critThreshold);
	void begin();
	void requestTemperatures();
	bool checkDataReady();
	bool loop(unsigned long counter);
private:
	DT* dt;
	OneWire* wire;
	void init();

};

