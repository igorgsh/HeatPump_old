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
	TempSensor(String label, int pin, float lowerRange, float upperRange);
	void begin();
	void requestTemperatures();
	bool checkDataReady();
	bool loop();

	//ActionStatus checkStatus();
private:
	DT* dt;
	OneWire* wire;
	void init();
	int tryCounter = 0;

};

