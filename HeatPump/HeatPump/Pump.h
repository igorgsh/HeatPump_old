#pragma once
#include "Relay.h"
#include "OutputDevice.h"

class Pump : 
	public OutputDevice
{
public:
	Pump(String label, int pin, bool on, unsigned long minTimeOn, unsigned long minTimeOff);
	~Pump();
	void begin();
	void StopPump();
	void StartPump();
private:
	Relay* r;
//	int minTimeOn = 0;
//	int minTimeOff = 0;
	//void Start();
	//void Stop();
//	bool DeviceCommand(ScenarioCommand cmd);
};

