#pragma once
#include "OutputDevice.h"
#include "Relay.h"

class Compressor :
	public Relay
{
public:
	Compressor(String label, int pin, bool on, unsigned long minTimeOn, unsigned long minTimeOff);
	~Compressor();
	void StopCompressor();
	void StartCompressor();
	void begin();
private:

};

