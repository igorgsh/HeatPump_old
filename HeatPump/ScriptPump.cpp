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

bool ScriptPump::IsStopAllowed(bool isSync) {
	//Nothing to do
	stopAlarm = 0;
	return false;
}

bool ScriptPump::IsStartAllowed(bool isSync) {
	bool alarm = false;
	bool res = true;

	switch (pump->GetPumpType()) {
	case PumpType::PUMP_GEO: {
		alarm = !((Config.DevMgr.tGeoI->getActionStatus() == ActionStatus::ACTION_NORMAL)
			& (Config.DevMgr.tGeoO->getActionStatus() == ActionStatus::ACTION_NORMAL));

		break;
	}
	default:
		alarm = false;
	}

	if (alarm) {
		if (isSync) {
			delay(alarmDelay * 1000);
			startAlarm = 0;
			res = false;
		}
		else {
			startAlarm = Config.counter1s; //set the timestamp with new counter
			res = true;
		}
	}
	else { //alarm is disappeared
		if (startAlarm != 0) {
			if (Config.counter1s >= startAlarm + alarmDelay) { // ready to go!
				startAlarm = 0;
				res = true;
			}
			else {//still waiting for delay...
				res = false;
			}
		}
		else {
			res = true;
		}
	}
	return res;
}

bool ScriptPump::Start(bool isSync) {
	bool res = false;

	if (pump->status == DeviceStatus::STATUS_ON) {
		res = true;
	}
	else {
		if (IsStartAllowed(isSync)) {
			long waitingTime = Config.counter1s - (pump->lastStatusTimestamp + pump->minTimeOff);

			if (waitingTime < 0) {//not ready to start
				if (isSync) {
					Debug("Start Pump delay:" + String(-waitingTime));
					delay(-waitingTime * 1000);
					waitingTime = 0;
				}
				else {
					res = false;
				}
			}
			if (waitingTime >= 0) { //ready to start
				if (IsStartAllowed(isSync)) { // no alarm occured
					pump->StartPump();
					pump->status = DeviceStatus::STATUS_ON;
					res = true;
				}
				else {
					res = false;
				}

			}
		}
	}
	return res;
}

bool ScriptPump::Stop(bool isSync) {
	bool res = false;
	if (pump->status == DeviceStatus::STATUS_OFF) {
		res = true;
	}
	else {
		if (IsStopAllowed(isSync)) {
			long waitingTime = Config.counter1s - (pump->lastStatusTimestamp + pump->minTimeOn);

			if (waitingTime < 0) {//not ready to stop
				if (isSync) {
					Debug("Stop Pump delay:" + String(-waitingTime));
					delay(-waitingTime * 1000);
					waitingTime = 0;
				}
				else {
					res = false;
				}
			}

			if (waitingTime >= 0) { //ready to stop
				if (IsStopAllowed(isSync)) { // no alarm occured
					pump->StopPump();
					pump->status = DeviceStatus::STATUS_OFF;
					res = true;
				}
				else {
					res = false;
				}
			}
		}
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
	stopAlarm = 0;
	return res;
}


bool ScriptPump::ForceStart() {
	bool res = false;
	if (pump->status == DeviceStatus::STATUS_ON) {
		res = true;
	}
	else {
		pump->StartPump();
		res = true;
	}
	startAlarm = 0;
	return res;
}
