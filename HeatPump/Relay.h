#pragma once
#include "OutputDevice.h"

class Relay : 
	public OutputDevice
{
public:
	Relay(String label, int pin, UnitType relayType, bool on);
	~Relay();
	void connect();
	void disconnect();
	void begin();
private:
	int pin;
	bool on;
};

