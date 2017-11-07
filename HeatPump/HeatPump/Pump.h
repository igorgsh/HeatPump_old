#pragma once
#include "Relay.h"
#include "OutputDevice.h"

class Pump :
	public Relay
{
public:
	Pump(String label, int pin, bool on, unsigned long minTimeOn, unsigned long minTimeOff);
	~Pump();
	void begin();
	void StopPump();
	void StartPump();
private:
};

