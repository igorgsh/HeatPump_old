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
	if (status != DeviceStatus::STATUS_OFF) {
		disconnect();
		lastStatusTimestamp = Config.Counter1s;
		Config.GetMqttClient()->PublishRelay(this);
	}
}

void Pump::StartPump() {
	if (status != DeviceStatus::STATUS_ON) {
		connect();
		lastStatusTimestamp = Config.Counter1s;
		Config.GetMqttClient()->PublishRelay(this);
	}
}

void Pump::begin() {
	Relay::begin();
}
