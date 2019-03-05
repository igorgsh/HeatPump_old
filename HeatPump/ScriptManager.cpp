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

	scriptPumpGeo = new ScriptPump(Config.DevMgr.pumpGeo, true, "PG", 1); //TODO: set a real values for alarmdelay
	scriptPumpContour1 = new ScriptPump(Config.DevMgr.pumpContour1, true, "P1", 1);
	scriptPumpContour2 = new ScriptPump(Config.DevMgr.pumpContour2, true, "P2", 1);
	scriptHeatPump = new ScriptHeatPump(true,"C1", 1);

}



ScriptManager::~ScriptManager()
{
}



bool ScriptManager::setup() {
	bool res = true;
	Loger::Debug("Starting Pump 1");
	if (scriptPumpContour1->MainLoop(true)) {
		Loger::Debug("Starting Pump 2");
		if (!scriptPumpContour2->MainLoop(true)) {
			Loger::Debug("Pump2 didn't started!");
			res = false;
		}
	}
	else {
		Loger::Debug("Pump1 didn't started!");
		res = false;
	}
//	SetCmd(ScenarioCmd::SCENARIO_START);
	return res;
}



void ScriptManager::loop() {
	// Main loop for scenario run
	if (scriptHeatPump->IsAlarm()) {
		//Debug("ScriptManager#Alarm");
		if (scriptHeatPump->GetAlarm() == 0) { //It is a new Alarm
			Loger::Debug("ScriptManager#Alarm:Force Stop");
			scriptHeatPump->ForceStop();
		}
		scriptHeatPump->SetAlarm(Config.counter1s); //set the timestamp with new counter
	}
	else { //alarm is disappeared or not exists
		if (scriptHeatPump->GetAlarm()!= 0) { // alarm is disappeared. Delay is needed
			Loger::Debug("ScriptManager#No Alarm");

			if (Config.counter1s >= scriptHeatPump->GetAlarm() + scriptHeatPump->GetAlarmDelay()) { // ready to go!
				Loger::Debug("ScriptManager#Alarm finished");

				scriptHeatPump->SetAlarm(0);
				scriptHeatPump->SetStep(0);
			}
			else {//still waiting for delay...
				//nothing to do
			}
		}
		else { // no old alarm. Run main loop
			//Debug("ScriptManager#MainLoop");

			scriptHeatPump->MainLoop(true);
		}
	}
}


