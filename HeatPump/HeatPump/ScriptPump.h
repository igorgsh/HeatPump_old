#pragma once
#include "Scenario.h"
#include "Pump.h"
//#include "TempSensor.h"
#include "Definitions.h"

class ScriptPump :
	public Scenario
{
public:
	ScriptPump(Pump* p);
	~ScriptPump();
	ScenarioCmd TriggerredCmd();
private:
	Pump* pump;
};

