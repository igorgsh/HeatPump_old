#pragma once
#include "Relay.h"
#include "OutputDevice.h"

class Pump :
	public Relay
{
public:
	Pump(String label, UnitType pumpType, int pin, bool on, unsigned long minTimeOn=0, unsigned long minTimeOff=0);
	~Pump();
	void begin();
	void StopPump();
	void StartPump();

private:
};

