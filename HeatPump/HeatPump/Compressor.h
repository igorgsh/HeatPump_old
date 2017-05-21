#pragma once
#include "OutputDevice.h"
#include "Relay.h"

class Compressor :
	public OutputDevice
{
public:
	Compressor(String label, int pin, bool on, unsigned long minTimeOn, unsigned long minTimeOff);
	~Compressor();
	void begin();
	void StopCompressor();
	void StartCompressor();
private:
	Relay* r;

};

