#include "ScenarioManager.h"
#include "Configuration.h"

extern Configuration Config;

ScenarioManager::ScenarioManager(DeviceManager* DevMgr)
{
	this->DevMgr = DevMgr;
	init();
}

void ScenarioManager::init() {
	scripts[0] = new ScriptPump(DevMgr->pumpGeo, true, "PG");
	scriptPumpGeo = scripts[0];
	scripts[1] = new ScriptPump(DevMgr->pumpContour1, true, "P1");
	scriptPumpContour1 = scripts[1];
	scripts[2] = new ScriptPump(DevMgr->pumpContour2, true, "P2");
	scriptPumpContour2 = scripts[2];
	scripts[3] = new ScriptCompressor(&(DevMgr->compressor), true,"C1", scriptPumpGeo, scriptPumpContour1, scriptPumpContour2);
	scriptCompressor = scripts[3];
}


ScenarioManager::~ScenarioManager()
{
	for (int i = 0; i < NUMBER_OF_SCRIPTS; i++) {
		delete scripts[i];
	}
}

void ScenarioManager::loop1() {

}


void ScenarioManager::loop5() {
	static unsigned long tryStart = 0;

	// Main loop for scenario run

	// check triggers
	for (int i = 0; i < NUMBER_OF_SCRIPTS; i++) {
		if (scripts[i]->Enabled) {
			//Debug2("Script:", scripts[i]->getLabel());
			ScenarioCmd cmd = scripts[i]->TriggerredCmd();
			//Debug2("Cmd:", cmd);
			PrepareCmd(scripts[i], cmd);
		}
	}

	// Run scenario
	if (currentScript != NULL) {
		Debug2("CurrentScript:", currentScript->getLabel());
		Debug2("Current cmd:", currentCmd);
		Debug2("Try:", tryStart);
		bool res = currentScript->Run(currentCmd);
		Debug2("Result:", res);
		if (res) {//script is finished
			tryStart=Config.counter1s;
			currentScript = NULL;
			currentCmd = ScenarioCmd::SCENARIO_NOCMD;
		}
		else {//script is delayed
			if (Config.counter1s - tryStart >= HANGOUT_INTERVAL) { //forgot about script when it is running more than 10 minutes 
				Debug("Script is broken");
				tryStart = Config.counter1s;
				currentScript = NULL;
				currentCmd = ScenarioCmd::SCENARIO_NOCMD;
			}
		}
	}

}

void ScenarioManager::loop10() {

}

void ScenarioManager::PrepareCmd(Scenario* script, ScenarioCmd cmd) {
	//Debug("PrepareCmd");
	//Debug2("cmd=", cmd);
	//if (currentScript != NULL) {
	//	Debug2("CurrentScript=", currentScript->getLabel());
	//}
	if (cmd != ScenarioCmd::SCENARIO_NOCMD) {
		if (currentScript == NULL) {//There is no script is running right now
			Debug("Take command");
			currentScript = script;
			currentCmd = cmd;
		}
		else if (currentScript->getId() == script->getId()) {
			//Debug("Script is running");
			if (cmd == ScenarioCmd::SCENARIO_STOP) {//Stop has the highest priority
				currentCmd = cmd;
			} // else nothing to do
		} // current script is running
	}
	//Debug("End Prepare");
}

void ScenarioManager::begin() {
	//Debug("ScanMgr Begin");
	for (int i = 0; i < NUMBER_OF_SCRIPTS; i++) {
		//Debug2("Script:", scripts[i]->getLabel());
		scripts[i]->begin();
	}
}