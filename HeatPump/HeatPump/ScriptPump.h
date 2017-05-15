#pragma once
#include "Scenario.h"
#include "Pump.h"
//#include "TempSensor.h"
#include "Definitions.h"

class ScriptPump :
	public Scenario
{
public:
	ScriptPump(Pump* p, bool enable);
	~ScriptPump();
	ScenarioCmd TriggerredCmd();
	bool Run(ScenarioCmd cmd, unsigned long counter);

private:
	Pump* pump;
};

