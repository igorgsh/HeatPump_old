#include "Pump.h"
#include "Configuration.h"

extern Configuration Config;

Pump::Pump(String label, int pin, bool on, unsigned long minTimeOn, unsigned long minTimeOff) : 
	Relay(pin,on)
{
	status = DeviceStatus::STATUS_OFF;
	this->minTimeOn = minTimeOn;
	this->minTimeOff = minTimeOff;
	this->label = label;
}

Pump::~Pump()
{
}

void Pump::StopPump() {
	disconnect();
	lastStatusTimestamp = Config.counter1s;
}

void Pump::StartPump() {
	connect();
	lastStatusTimestamp = Config.counter1s;
}

void Pump::begin() {
	Relay::begin();
}
