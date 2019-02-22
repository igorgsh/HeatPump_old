#pragma once

#include "DeviceManager.h"
#include "Script.h"
//#include "ScriptPump.h"
//#include "ScriptHeatPump.h"

#define NUMBER_OF_SCRIPTS	4
//#define HANGOUT_INTERVAL	(10*60)
#define ALARM_DELAY_INTERVAL  5*60

class ScriptManager
{
public:
	ScriptManager();
	~ScriptManager();

	void loop();
	bool setup();
	void SetCmd(ScenarioCmd cmd);
	Script* scriptPumpGeo;
	Script *scriptPumpContour1;
	Script *scriptPumpContour2;
	Script* scriptHeatPump;

private:
	void init();
	ScenarioCmd lastCmd;
};

