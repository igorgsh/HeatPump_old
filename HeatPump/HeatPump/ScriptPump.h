#pragma once
#include "Scenario.h"
#include "Pump.h"
#include "Definitions.h"

class ScriptPump :
	public Scenario
{
public:
	ScriptPump(Pump* p, bool enable, String label);
	~ScriptPump();
	ScenarioCmd TriggerredCmd();
	bool Start();
	bool Stop();
	//bool Run(ScenarioCmd cmd, unsigned long counter) = 0;
	bool begin() { return true; };

private:
	Pump* pump;
};

