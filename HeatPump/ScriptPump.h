#pragma once
#include "Scenario.h"
#include "Pump.h"
#include "Definitions.h"

typedef enum {
	NO_ACTION=0,
	ALWAYS_START=1
} PumpMode;

class ScriptPump :
	public Scenario
{
public:
	ScriptPump(Pump* p, bool enable, String label, PumpMode pumpMode = PumpMode::NO_ACTION);
	~ScriptPump();
	ScenarioCmd TriggerredCmd();
	bool Start();
	bool Stop();
	bool begin() { return true; };

private:
	Pump* pump;
	PumpMode mode;
};

