#include "Pump.h"
#include "Configuration.h"

extern Configuration Config;

Pump::Pump(String label, PumpType pumpType, int pin, bool on, unsigned long minTimeOn, unsigned long minTimeOff) : 
	Relay(label, DeviceType::PUMP, pin,on)
{
	status = DeviceStatus::STATUS_OFF;
	pumpType = pumpType;
	this->minTimeOn = minTimeOn;
	this->minTimeOff = minTimeOff;
}

Pump::~Pump()
{
}

void Pump::StopPump() {
	disconnect();
	Config.MqttClient()->Publish(this, String(DeviceStatus::STATUS_OFF));

	lastStatusTimestamp = Config.counter1s;
}

void Pump::StartPump() {
	connect();
	Config.MqttClient()->Publish(this, String(DeviceStatus::STATUS_ON));

	lastStatusTimestamp = Config.counter1s;
}

void Pump::begin() {
	Relay::begin();
}
