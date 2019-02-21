#include "ScriptPump.h"
#include "Configuration.h"

extern Configuration Config;



ScriptPump::ScriptPump(Pump* p, bool enable, String label) : Script(enable, label, p)
{
	pump = p;
}


ScriptPump::~ScriptPump()
{
}

bool ScriptPump::Start(bool isSync) {
	bool res = false;
	if (pump->status == DeviceStatus::STATUS_ON) {
		res = true;
	}
	else {
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
	}
	else {
		long waitingTime = Config.counter1s - (pump->lastStatusTimestamp + pump->minTimeOn);

		if (waitingTime<0) {//not ready to stop
			if (isSync) {
				Debug("Stop Pump delay:" + String(-waitingTime));
				delay(-waitingTime * 1000);
				waitingTime = 0;
			}
			else {
				res = false;
			}
		}
		if (waitingTime >= 0) { //ready to start
			pump->StopPump();
			res = true;
		}
	}
	return res;
}
