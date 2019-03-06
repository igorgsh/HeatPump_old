#pragma once
#include "Relay.h"
#include "OutputDevice.h"

typedef enum {
	PUMP_NO_PUMP = 0,
	PUMP_CONTOUR1 = 1,
	PUMP_CONTOUR2 = 2,
	PUMP_GEO = 3,
	PUMP_FLOOR = 4
} PumpType;

class Pump :
	public Relay
{
public:
	Pump(String label, PumpType pumpType, int pin, bool on, unsigned long minTimeOn, unsigned long minTimeOff);
	~Pump();
	void begin();
	void StopPump();
	void StartPump();
	PumpType GetPumpType() { return pumpType; }

private:
	PumpType pumpType = PUMP_NO_PUMP;
};

