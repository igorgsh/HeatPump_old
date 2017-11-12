#include "ScenarioManager.h"
#include "Configuration.h"

extern Configuration Config;

ScenarioManager::ScenarioManager(DeviceManager* DevMgr)
{
	this->DevMgr = DevMgr;
	init();
}

void ScenarioManager::init() {
	scripts[0] = new ScriptPump(DevMgr->pumpGeo, true, "PG",PumpMode::NO_ACTION);
	scriptPumpGeo = scripts[0];
	scripts[1] = new ScriptPump(DevMgr->pumpContour1, true, "P1", PumpMode::ALWAYS_START);
	scriptPumpContour1 = scripts[1];
	scripts[2] = new ScriptPump(DevMgr->pumpContour2, true, "P2", PumpMode::ALWAYS_START);
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



void ScenarioManager::loop() {
	static unsigned long tryStart = 0;

	// Main loop for scenario run
	// check triggers
	for (int i = 0; i < NUMBER_OF_SCRIPTS; i++) {
		if (scripts[i]->Enabled) {
			ScenarioCmd cmd = scripts[i]->TriggerredCmd();
			//Debug2("Cmd:", cmd);
			//PrepareCmd(scripts[i], cmd);
			//Debug("Counter=" + String(Config.counter1s));
			//Debug2("CurrentScript:", scripts[i]->getLabel());
			//Debug2("Current cmd:", cmd);
			//Debug2("Try:", tryStart);
			bool res = scripts[i]->Run(cmd);
			//Debug2("Result:", res);
			if (res) {//script is finished
				//Debug("Script" + scripts[i]->getLabel() + " is finished");
				tryStart = Config.counter1s;
				scripts[i]->step = 0;
				//script[i] = NULL;
				//currentCmd = ScenarioCmd::SCENARIO_NOCMD;
			}
			else {//script is not finished yet
				if (Config.counter1s - tryStart >= HANGOUT_INTERVAL) { //stop about script when it is running more than 10 minutes 
					Debug("Script " + scripts[i]->getLabel() + " is hanging out");
					tryStart = Config.counter1s;
					scripts[i]->step = 0;
					//currentScript = NULL;
					//currentCmd = ScenarioCmd::SCENARIO_NOCMD;
				}
			}
		}
	}
}

/*
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
*/


void ScenarioManager::begin() {
	//Debug("ScanMgr Begin");
	for (int i = 0; i < NUMBER_OF_SCRIPTS; i++) {
		//Debug2("Script:", scripts[i]->getLabel());
		scripts[i]->begin();
	}
}