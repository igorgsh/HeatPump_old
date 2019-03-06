#include "Compressor.h"
#include "Arduino.h"
#include "Configuration.h"

extern Configuration Config;

Compressor::Compressor(String label, int pin, bool on, unsigned long minTimeOn, unsigned long minTimeOff) :
	Relay(label, DeviceType::COMPRESSOR, pin, on)
{
	this->minTimeOn = minTimeOn;
	this->minTimeOff = minTimeOff;
}

Compressor::~Compressor()
{
}

bool Compressor::StopCompressor() {
	Loger::Debug("Stop Compressor!!!");
	disconnect();
	Config.MqttClient()->Publish(this, String(DeviceStatus::STATUS_OFF));
	lastStatusTimestamp = Config.counter1s;
	return true;
}

bool Compressor::StartCompressor() {
	Loger::Debug("Start Compressor!!!");
	connect();
	Config.MqttClient()->Publish(this, String(DeviceStatus::STATUS_ON));
	lastStatusTimestamp = Config.counter1s;
	return true;
}

void Compressor::begin() {
	Relay::begin();
}

