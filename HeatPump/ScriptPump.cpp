#include "ScriptPump.h"
#include "Configuration.h"

extern Configuration Config;



ScriptPump::ScriptPump(Pump* p, bool enable, String label, unsigned int alarmDelay) : Script(enable, label, alarmDelay)
{
	pump = p;
}


ScriptPump::~ScriptPump()
{
}


bool ScriptPump::IsAlarm() {
	bool res = true;

	switch (pump->GetPumpType()) {
	case PumpType::PUMP_GEO: {
		res = !((Config.DevMgr.tGeoI->getActionStatus() == ActionStatus::ACTION_NORMAL)
			& (Config.DevMgr.tGeoO->getActionStatus() == ActionStatus::ACTION_NORMAL));

		break;
	}
	default:
		res = false;
	}
	return res;
}

bool ScriptPump::Start(bool isSync) {
	bool res = false;

	if (pump->status == DeviceStatus::STATUS_ON) {
		res = true;
	}
	else {
		if (!IsAlarm()) {
			pump->StartPump();
			pump->status = DeviceStatus::STATUS_ON;
			res = true;
		}
	}
	return res;
}

bool ScriptPump::Stop(bool isSync) {
	bool res = false;
	if (pump->status == DeviceStatus::STATUS_OFF) {
		res = true;
		Debug("Pump Stopped(OFF)!");
	}
	else {
		pump->StopPump();
		pump->status = DeviceStatus::STATUS_OFF;
		res = true;
	}
	return res;
}

bool ScriptPump::ForceStop() {
	bool res = false;
	if (pump->status == DeviceStatus::STATUS_OFF) {
		res = true;
	}
	else {
		pump->StopPump();
		res = true;
	}
	return res;
}

