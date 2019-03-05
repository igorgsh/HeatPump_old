#include "Compressor.h"
#include "Arduino.h"
#include "Configuration.h"

extern Configuration Config;

Compressor::Compressor(String label, int pin, bool on, unsigned long minTimeOn, unsigned long minTimeOff) :
	Relay(pin, on)
{
	this->minTimeOn = minTimeOn;
	this->minTimeOff = minTimeOff;
	this->label = label;
}

Compressor::~Compressor()
{
}

bool Compressor::StopCompressor() {
	Loger::Debug("Stop Compressor!!!");
	disconnect();
	lastStatusTimestamp = Config.counter1s;
	return true;
}

bool Compressor::StartCompressor() {
	Loger::Debug("Start Compressor!!!");
	connect();
	lastStatusTimestamp = Config.counter1s;
	return true;
}

void Compressor::begin() {
	Relay::begin();
}

