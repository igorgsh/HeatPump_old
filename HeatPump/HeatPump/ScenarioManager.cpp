#include "ScenarioManager.h"



ScenarioManager::ScenarioManager(DeviceManager* DevMgr)
{
	this->DevMgr = DevMgr;
}


ScenarioManager::~ScenarioManager()
{
}

void ScenarioManager::loop1(unsigned long counter) {

}


void ScenarioManager::loop5(unsigned long counter) {
	static int tryCounter = 0;

	// Main loop for scenario run

	// check triggers
	for (int i = 0; i < NUMBER_OF_SCRIPTS; i++) {
		if (scripts[i].Enabled) {
			Debug2("Script:", i);
			ScenarioCmd cmd = scripts[i].TriggerredCmd();
			Debug2("Cmd:", cmd);
			PrepareCmd(&(scripts[i]), cmd);
		}
	}

	// Run scenario
	if (currentScript != NULL) {
		Debug2("CurrentSprint:", currentScript->getLabel());
		Debug2("Current cmd:", currentCmd);
		Debug2("Try:", tryCounter);
		bool res = currentScript->Run(currentCmd,counter);
		Debug2("Result:", res);
		if (res) {//script is finished
			tryCounter = 0;
			currentScript = NULL;
			currentCmd = ScenarioCmd::SCENARIO_NOCMD;
		}
		else {//script is delayed
			tryCounter++;
			if (tryCounter >= 10 * 60 * 2) { //forgot about script when it is running more than 10 minutes 
				Debug("Script is broken");
				tryCounter = 0;
				currentScript = NULL;
				currentCmd = ScenarioCmd::SCENARIO_NOCMD;
			}
		}
	}

}

void ScenarioManager::loop10(unsigned long counter) {

}

void ScenarioManager::PrepareCmd(Scenario* script, ScenarioCmd cmd) {
	if (cmd != ScenarioCmd::SCENARIO_NOCMD) {
		if (currentScript != NULL) {//There is no script is running right now
			currentScript = script;
			currentCmd = cmd;
		}
		else if (currentScript->getId() == script->getId()) {
			if (cmd == ScenarioCmd::SCENARIO_STOP) {//Stop has the highest priority
				currentCmd = cmd;
			} // else nothing to do
		} // current script is running
	}
}

