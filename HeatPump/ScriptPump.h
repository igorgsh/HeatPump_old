#pragma once
#include "Script.h"
#include "Pump.h"
#include "Definitions.h"

typedef enum {
	NO_ACTION=0,
	ALWAYS_START=1
} PumpMode;

class ScriptPump :
	public Script
{
public:
	ScriptPump(Pump* p, bool enable, String label, unsigned int alarmDelay);
	~ScriptPump();
	bool ForceStart();
	bool Start(bool isSync);
	bool Stop(bool isSync);
	bool ForceStop();
	bool MainLoop(bool isSync) { return Start(isSync); }
	bool IsAlarm();
	//bool begin() { return true; };
	//bool IsStartAllowed(bool isSync);
	//bool IsStopAllowed(bool isSync);

private:
	Pump* pump;
};

