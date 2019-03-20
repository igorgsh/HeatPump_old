#pragma once
#include "Sensor.h"
//#include "DT.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#define DT_DEFAULT_RESOLUTION 10


class TempSensorSingle :
	public Sensor
{
public:
	~TempSensorSingle();
	TempSensorSingle(String label, int pin, float lowerRange, float upperRange);
	void begin();
	bool checkDataReady();
	bool loop();
	//ActionStatus checkStatus();
private:
	//DT* dt;
	DallasTemperature* dt;
	OneWire* wire;
	void init();
	int tryCounter = 0;
	void requestTemperatures();

};

