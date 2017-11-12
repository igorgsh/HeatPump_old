#include "ScriptPump.h"
#include "Configuration.h"

extern Configuration Config;



ScriptPump::ScriptPump(Pump* p, bool enable, String label, PumpMode pumpMode) : Scenario(enable, label, p)
{
	pump = p;
	mode = pumpMode;
}


ScriptPump::~ScriptPump()
{
}

ScenarioCmd ScriptPump::TriggerredCmd() {
	ScenarioCmd res = ScenarioCmd::SCENARIO_NOCMD;
	switch (mode) {
	case ALWAYS_START: {
		if (pump->status != DeviceStatus::STATUS_ON) {
			res = ScenarioCmd::SCENARIO_START;
		}
		else {
			res = ScenarioCmd::SCENARIO_NOCMD;
		}
		break;
	}
	default: {
		res = ScenarioCmd::SCENARIO_NOCMD;
		break;
	}
	}
	return res;
}

bool ScriptPump::Start() {
	bool res;
	if (pump->status == DeviceStatus::STATUS_ON) {
		res = true;
	}
	else {
		if (pump->lastStatusTimestamp + pump->minTimeOff <= Config.counter1s) { //device is off a long time
			pump->StartPump();
			res = true;
		}
		else {
			res = false;
		}
	}
	return res;
}

bool ScriptPump::Stop() {
	bool res;
	if (pump->status == DeviceStatus::STATUS_OFF) {
		res = true;
	}
	else {
		if (pump->lastStatusTimestamp + pump->minTimeOn <= Config.counter1s) { //device is on a long time
			pump->StopPump();
			res = true;
		}
		else {
			res = false;
		}
	}
	return res;
}
