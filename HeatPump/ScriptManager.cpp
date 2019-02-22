#include "ScriptManager.h"
#include "Configuration.h"
#include "ScriptPump.h"
#include "ScriptHeatPump.h"

extern Configuration Config;

ScriptManager::ScriptManager()
{
	init();
}

void ScriptManager::init() {

	scriptPumpGeo = new ScriptPump(Config.DevMgr.pumpGeo, true, "PG", 3 * 60);
	scriptPumpContour1 = new ScriptPump(Config.DevMgr.pumpContour1, true, "P1", 3 * 60);
	scriptPumpContour2 = new ScriptPump(Config.DevMgr.pumpContour2, true, "P2", 3 * 60);
	scriptHeatPump = new ScriptHeatPump(true,"C1", 3 * 60);

}



ScriptManager::~ScriptManager()
{
}


bool ScriptManager::setup() {
	bool res = true;
	Debug("Starting Pump 1");
	if (scriptPumpContour1->Start(true)) {
		Debug("Starting Pump 2");
		if (!scriptPumpContour2->Start(true)) {
			Debug("Pump2 didn't started!");
			res = false;
		}
	}
	else {
		Debug("Pump1 didn't started!");
		res = false;
	}
	SetCmd(ScenarioCmd::SCENARIO_START);
	return res;
}



void ScriptManager::loop() {
	// Main loop for scenario run
	scriptHeatPump->Run(lastCmd);
}

void ScriptManager::SetCmd(ScenarioCmd cmd) {
	lastCmd = cmd;
}


