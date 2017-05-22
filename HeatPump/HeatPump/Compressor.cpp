#include "Compressor.h"
#include "Arduino.h"
#include "Configuration.h"

extern Configuration Config;

Compressor::Compressor(String label, int pin, bool on, unsigned long minTimeOn, unsigned long minTimeOff)
{
	r = new Relay(pin, on);
	this->minTimeOn = minTimeOn;
	this->minTimeOff = minTimeOff;
	this->label = label;
}

Compressor::~Compressor()
{
}

void Compressor::StopCompressor() {
	r->disconnect();
	lastStatusTimestamp = Config.counter1;
}

void Compressor::StartCompressor() {
	Debug("Start Compressor!!!");
	r->connect();
	lastStatusTimestamp = Config.counter1;
}

void Compressor::begin() {
	r->begin();
}

