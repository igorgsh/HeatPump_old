#include "ScriptPump.h"



ScriptPump::ScriptPump(Pump* p, bool enable) : Scenario(enable, "")
{
	pump = p;
}


ScriptPump::~ScriptPump()
{
}

ScenarioCmd ScriptPump::TriggerredCmd() {
	return ScenarioCmd::SCENARIO_NOCMD;
}

bool ScriptPump::Run(ScenarioCmd cmd, unsigned long counter) {

	bool res = false;
	if (cmd == ScenarioCmd::SCENARIO_START) {
		if (pump->status == STATUS_ON) { // Nothing to do. Pump is running 
			res = true;
		}
		else { //Pump is off of unknown
			if (pump->lastStatusTimestamp + pump->minTimeOff <= counter) { //Pump is off a long time. Start it
				pump->StartPump();
				res = true;
			}
			else { //Pump is Off but timeout is still not over
				res = false;
			}
		}
	}
	else if (cmd == ScenarioCmd::SCENARIO_STOP) {
		if (pump->status == STATUS_OFF) { // Nothing to do. Pump is stop 
			res = true;
		}
		else { //Pump is on of unknown
			if (pump->lastStatusTimestamp + pump->minTimeOn <= counter) { //Pump is on a long time. Stop it
				pump->StopPump();
				res = true;
			}
			else { //Pump is On but timeout is still not over
				res = false;
			}
		}
	} else {//unknown command
		res = true;
	}
	return res;
}

