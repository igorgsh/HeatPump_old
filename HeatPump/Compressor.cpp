#include "Compressor.h"
#include "Arduino.h"
#include "Configuration.h"
#include "Loger.h"

extern Configuration Config;

Compressor::Compressor(String label, int pin, bool on, unsigned long minTimeOn, unsigned long minTimeOff) :
	Relay(label, pin, UnitType::UT_Compressor, on)
{
	this->minTimeOn = minTimeOn;
	this->minTimeOff = minTimeOff;
}

Compressor::~Compressor()
{
}

bool Compressor::StopCompressor() {
	if (status != DeviceStatus::STATUS_OFF) {
		Loger::Debug("Stop Compressor!!!");
		disconnect();
		lastStatusTimestamp = Config.Counter1s;
		Config.GetMqttClient()->PublishRelay(this);
	}
	return true;
}

bool Compressor::StartCompressor() {
	if (status != DeviceStatus::STATUS_ON) {
		Loger::Debug("Start Compressor!!!");
		connect();
		lastStatusTimestamp = Config.Counter1s;
		Config.GetMqttClient()->PublishRelay(this);
	}
	return true;
}

void Compressor::begin() {
	Relay::begin();
}

