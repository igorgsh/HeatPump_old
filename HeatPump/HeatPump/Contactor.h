#pragma once
#include "Sensor.h"
class Contactor :
	public Sensor
{
public:
	~Contactor();

	Contactor(String label, int pin, bool lhOn, float actionPoints[], int critThreshold);
	void begin();
//	void requestTemperatures();
	bool checkDataReady();
	bool loop(unsigned long counter);
	//ActionStatus getActionStatus() { return actionStatus; };

private:
	bool lhOn;
	void init();

};

