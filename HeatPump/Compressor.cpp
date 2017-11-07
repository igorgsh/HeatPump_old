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

void Compressor::StopCompressor() {
	disconnect();
	lastStatusTimestamp = Config.counter1s;
}

void Compressor::StartCompressor() {
	Debug("Start Compressor!!!");
	connect();
	lastStatusTimestamp = Config.counter1s;
}

void Compressor::begin() {
	Relay::begin();
}

