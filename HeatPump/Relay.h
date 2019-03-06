#pragma once
#include "OutputDevice.h"
class Relay : 
	public OutputDevice
{
public:
	Relay(String lbl, DeviceType tp, int pin, bool on);
	~Relay();
	void connect();
	void disconnect();
	void begin();
private:
	int pin;
	bool on;
};

