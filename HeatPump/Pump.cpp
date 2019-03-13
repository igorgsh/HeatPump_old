#include "Pump.h"
#include "Configuration.h"

extern Configuration Config;

Pump::Pump(String label, UnitType pumpType, int pin, bool on, unsigned long minTimeOn, unsigned long minTimeOff) : 
	Relay(label, pin, pumpType, on)
{
	status = DeviceStatus::STATUS_OFF;
	this->minTimeOn = minTimeOn;
	this->minTimeOff = minTimeOff;
}

Pump::~Pump()
{
}

void Pump::StopPump() {
	disconnect();
	lastStatusTimestamp = Config.Counter1s;
}

void Pump::StartPump() {
	connect();
	lastStatusTimestamp = Config.Counter1s;
}

void Pump::begin() {
	Relay::begin();
}
