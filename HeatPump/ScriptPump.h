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
	ScriptPump(Pump* p, bool enable, String label);
	~ScriptPump();
	//ScenarioCmd TriggerredCmd();
	bool Start(bool isSync);
	bool Stop(bool isSync);
	bool begin() { return true; };

private:
	Pump* pump;
	//PumpMode mode;
};

