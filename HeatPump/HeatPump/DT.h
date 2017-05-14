#pragma once
#include "DallasTemperature.h"
class DT :
	public DallasTemperature
{
public:
	DT(OneWire* _wire) : DallasTemperature(_wire) {};
	~DT();
	void begin();
	// finds an address at a given index on the bus
	bool getAddress(uint8_t*, uint8_t);
	bool isConversionAvailable(int index);

private:
	DeviceAddress* addresses;
};

