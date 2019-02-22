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

void ScriptPump::CheckStopAlarm(bool isSync) {
	//Nothing to do
	stopAlarm = 0;
}

void ScriptPump::CheckStartAlarm(bool isSync) {
	bool alarm = false;
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
		}
		else {
			startAlarm = Config.counter1s; //set the timestamp with new counter
		}
	}
	else { //alarm is disappeared
		if (Config.counter1s >= startAlarm + alarmDelay) { // ready to go!
			startAlarm = 0;
		}
		else {//still waiting for delay...

		}
	}
}

bool ScriptPump::Start(bool isSync) {
	bool res = false;

	if (pump->status == DeviceStatus::STATUS_ON) {
		res = true;
	}
	else {
		CheckStartAlarm(isSync);
		if (!IsStartAlarm()) {
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
				CheckStartAlarm(isSync);
				if (!IsStartAlarm()) { // Some alarm occured
					pump->StartPump();
					pump->status = DeviceStatus::STATUS_ON;
					res = true;
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
		CheckStopAlarm(isSync);
		if (!IsStopAlarm()) {
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
				CheckStopAlarm(isSync);
				if (!IsStopAlarm()) { // Some alarm occured
					pump->StopPump();
					pump->status = DeviceStatus::STATUS_OFF;
					res = true;
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
