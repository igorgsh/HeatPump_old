#pragma once
#include "Sensor.h"
class Contactor :
	public Sensor
{
public:
	~Contactor();

	Contactor(String label, int pin, bool lhOn, bool alarmOn);
	void begin();
	bool checkDataReady();
	bool loop();
	ActionStatus checkStatus();

private:
	bool lhOn;
	void init();
	bool AlarmOn;

};

