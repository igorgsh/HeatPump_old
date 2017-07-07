#pragma once
#include "Sensor.h"
class Contactor :
	public Sensor
{
public:
	~Contactor();
	Contactor(String label, int pin, bool alarm, int critThreshold);
	void begin();
//	void requestTemperatures();
	bool checkDataReady();
	bool loop(unsigned long counter);
private:
	bool Alarm;
	void init();

};

