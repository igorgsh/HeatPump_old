#pragma once
#include "OutputDevice.h"
#include "Relay.h"

class Compressor :
	public Relay
{
public:
	Compressor(String label, int pin, bool on, unsigned long minTimeOn, unsigned long minTimeOff);
	~Compressor();
	bool StopCompressor();
	bool StartCompressor();
	void begin();
private:

};

