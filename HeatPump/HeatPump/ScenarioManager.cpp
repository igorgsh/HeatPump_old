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
	static int cnt = 0;

	// Main loop for scenario run

	// check triggers
	for (int i = 0; i < NUMBER_OF_SCRIPTS; i++) {
		if (scripts[i].Enabled) {
			ScenarioCmd cmd = scripts[i].TriggerredCmd();
			Run(&(scripts[i]), cmd);
		}
	}

	// Run scenario
	if (currentScript != NULL) {
		bool res = currentScript->Run(currentCmd,counter);
		if (res) {//script is finished
			cnt = 0;
			currentScript = NULL;
			currentCmd = ScenarioCmd::SCENARIO_NOCMD;
		}
		else {//script is delayed
			cnt++;
			if (cnt >= 10 * 60 * 2) { //forgot about script when it is running more than 10 minutes 
				cnt = 0;
				currentScript = NULL;
				currentCmd = ScenarioCmd::SCENARIO_NOCMD;
			}
		}
	}

}

void ScenarioManager::loop10(unsigned long counter) {

}

void ScenarioManager::Run(Scenario* script, ScenarioCmd cmd) {
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

