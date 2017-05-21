#pragma once
#include "OutputDevice.h"
class Relay : 
	public OutputDevice
{
public:
	Relay(int pin, bool on);
	~Relay();
	void connect();
	void disconnect();
	void begin();
	//bool DeviceCommand(ScenarioCommand cmd);
//	bool status() { return status; };
private:
	int pin;
	bool on;
//	bool status;
};

