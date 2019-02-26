#include "Configuration.h"
#include "ScriptPump.h"
#include "ScriptHeatPump.h"

extern Configuration Config;

ScriptHeatPump::ScriptHeatPump(bool enable, String label, unsigned int alarmDelay) : Script(enable, label, alarmDelay)
{
	this->step = COMPRESSOR_IDLE;
}


ScriptHeatPump::~ScriptHeatPump()
{
}

bool ScriptHeatPump::checkInternalTempConditions() {
	bool ret = true;
	ret &= (Config.DevMgr.tGeoI->getActionStatus() == ActionStatus::ACTION_NORMAL);
	ret &= (Config.DevMgr.tGeoO->getActionStatus() == ActionStatus::ACTION_NORMAL);
	ret &= (Config.DevMgr.tEvoI->getActionStatus() == ActionStatus::ACTION_NORMAL);
	ret &= (Config.DevMgr.tEvoO->getActionStatus() == ActionStatus::ACTION_NORMAL);
	ret &= (Config.DevMgr.tCondI->getActionStatus() == ActionStatus::ACTION_NORMAL);
	ret &= (Config.DevMgr.tCondO->getActionStatus() == ActionStatus::ACTION_NORMAL);
	ret &= (Config.DevMgr.tHpO->getActionStatus() == ActionStatus::ACTION_NORMAL);
	ret &= (Config.DevMgr.tHpI->getActionStatus() == ActionStatus::ACTION_NORMAL);
	ret &= (Config.DevMgr.tComp->getActionStatus() == ActionStatus::ACTION_NORMAL);
//	Debug2("Check Internal Temp Conditions=", ret);
	return ret;
}
bool ScriptHeatPump::checkContactors() {
	bool ret = true;
	ret &= (Config.DevMgr.cFlow->getActionStatus() == ActionStatus::ACTION_NORMAL);
	//Debug2("CheckContactor=", ret);
	return ret;
}

bool ScriptHeatPump::IsStopNeeded() {
	bool res;

	res = checkAllConditions();
	if (res) {
		res = (Config.ControlTemperature() >= Config.OutTemperature());
		//	Debug2("IsStartNeeded=", res);
	}
	else {
		res = true;
		Debug("Compressor: Force Stop!");
	}
	return res;
}

bool ScriptHeatPump::IsStartNeeded() {
	bool res;

	res = (Config.ControlTemperature() < Config.getDesiredTemp());
	//	Debug2("IsStartNeeded=", res);
	return res;
}


bool ScriptHeatPump::checkAllConditions() {
	return checkInternalTempConditions()
		&& checkContactors();
}

void ScriptHeatPump::CheckStopAlarm(bool isSync) {
	stopAlarm = 0;
}

void ScriptHeatPump::CheckStartAlarm(bool isSync) {
	
	bool alarm = false;
	
	alarm = checkAllConditions();

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

bool ScriptHeatPump::Start(bool isSync) {

	//Debug("Compressor Start:" + String(step));
	bool res = false;
	// Step 0: Is start needed
	// Step 1: start Pump Geo
	// Step 2: Waiting for 3 min
	// Step 3: Start Compressor
	// Step 4: Waiting for desired temp achieved
	// Step 5: Stop Compressor
	// Step 6: Waiting for 3 min
	// Step 7: stop Pump Geo
	// Finish
	
	if (step == COMPRESSOR_IDLE) { //Nothing to do
		step = COMPRESSOR_IS_START_NEEDED;
		Debug("The Compressor is starting");

	}
	if (step == COMPRESSOR_IS_START_NEEDED) { // Is Start Needed
		
		if (IsStartNeeded()) {
			Debug("Compressor: Start is needed");
			bool res1 = Config.ScriptMgr.scriptPumpContour1->Start(isSync);
			bool res2 = Config.ScriptMgr.scriptPumpContour2->Start(isSync);
			Debug("Pump contours:" + String(res1) + "#" + String(res2));
			if (res1 && res2) {
				step = COMPRESSOR_START_PUMP_GEO;
				Debug("Compressor: Geo pump starting!");
			}
		}
	}
	if (step == COMPRESSOR_START_PUMP_GEO) { // Start Pump Geo
		if (Config.ScriptMgr.scriptPumpGeo->Start(isSync)) {
			Debug("Compressor: Geo pump is done!");
			step = COMPRESSOR_WAITING_PUMP_GEO_START;
			counterScript = Config.counter1s;
		}
	}
	if (step == COMPRESSOR_WAITING_PUMP_GEO_START) {//delay before start compressor
		if (Config.counter1s - counterScript >= COMPRESSOR_ON_TIMEOUT) {
			step = COMPRESSOR_START;
			Debug("Compressor is starting...");
		}
	}
	if (step == COMPRESSOR_START) { //Start Compressor
		if (StartCompressor(isSync)) {
			step = COMPRESSOR_IS_STOP_NEEDED;
			Debug("Compressor: Started!");
		}
	}
	if (step == COMPRESSOR_IS_STOP_NEEDED) { // Is the temp achieved?
//		Debug("Compressor: is stop needed?");
		if (IsStopNeeded()) {
			step = COMPRESSOR_STOP;
			Debug("Compressor: Stop!");
		}
	}
	if (step == COMPRESSOR_STOP) { // Stop Compressor
		Debug("Compressor: Stop!");
		if (Config.DevMgr.compressor.StopCompressor()) {
			Debug("Compressor: Stop done!");
			step = COMPRESSOR_WAITING_PUMP_GEO_STOP;
			counterScript = Config.counter1s;
			Debug("Compressor: Waiting before Geo Pump stop");
		}
	}
	if (step == COMPRESSOR_WAITING_PUMP_GEO_STOP) {//delay before stop Pump Geo
		if (Config.counter1s - counterScript >= PUMP_OFF_TIMEOUT) {
			step = COMPRESSOR_STOP_PUMP_GEO;
			Debug("Compressor:Stop Geo Pump ");
		}
	}
	if (step == COMPRESSOR_STOP_PUMP_GEO) { // Stop Pump Geo
		if (Config.ScriptMgr.scriptPumpGeo->Stop(false)) {
			step = COMPRESSOR_IS_START_NEEDED;
			res = true;
		}
	}
		//Debug2("Return:", String(res));
	return res;
}

bool ScriptHeatPump::Stop(bool isSync) {
	bool res = false;
	if (step == COMPRESSOR_IDLE
		|| step == COMPRESSOR_IS_START_NEEDED
		) { //Nothing to do
		step = COMPRESSOR_IDLE;
		res = true;
	}
	if (step == COMPRESSOR_START_PUMP_GEO
		|| step == COMPRESSOR_WAITING_PUMP_GEO_START
		|| step == COMPRESSOR_WAITING_PUMP_GEO_STOP
		|| step == COMPRESSOR_STOP_PUMP_GEO
		) { // Start Pump Geo
		if (Config.ScriptMgr.scriptPumpGeo->Stop(isSync)) {
			step = COMPRESSOR_IDLE;
			res = true;
			counterScript = Config.counter1s;
		}
	}
	if (step == COMPRESSOR_START
		|| step == COMPRESSOR_IS_STOP_NEEDED
		|| step == COMPRESSOR_STOP
		) { //Start Compressor
		if (StopCompressor(isSync)) {
			step = COMPRESSOR_STOP_PUMP_GEO;
		}
	}
	//Debug2("Return:", String(res));
	return res;

}


bool ScriptHeatPump::StartCompressor(bool isSync) {
	bool res = false;
	if (Config.DevMgr.compressor.status == DeviceStatus::STATUS_ON) {
		res = true;
	}
	else {

		if (checkAllConditions()) {
			long waitingTime = Config.counter1s - (Config.DevMgr.compressor.lastStatusTimestamp + Config.DevMgr.compressor.minTimeOff);
			if (waitingTime < 0) {//not ready to start
				if (isSync) {
					Debug("Start Compressor delay:" + String(-waitingTime));
					delay(-waitingTime * 1000);
					waitingTime = 0;
				}
				else {
					res = false;
				}
			}
			if (waitingTime > 0) { //ready to start
				if (checkAllConditions()) {
					Config.DevMgr.compressor.StartCompressor();
					Config.DevMgr.compressor.status = DeviceStatus::STATUS_ON;
					res = true;
				}
				else {
					Debug("Critical conditions are achieved!");
					res = false;
				}
			}
		}
	}
	return res;
}

bool ScriptHeatPump::StopCompressor(bool isSync) {
	bool res = false;
	if (Config.DevMgr.compressor.status == DeviceStatus::STATUS_OFF) {
		res = true;
	}
	else {
		long waitingTime = Config.counter1s - (Config.DevMgr.compressor.lastStatusTimestamp + Config.DevMgr.compressor.minTimeOn);

		if (waitingTime > 0) { //ready to start
			Config.DevMgr.compressor.StopCompressor();
			res = true;
		}
		else {
			if (isSync) {
				delay(waitingTime * 1000);
			}
			else {
				res = false;
			}
		}
	}
	return res;
}

bool ScriptHeatPump::ForceStop() {
	bool res = false;
	if (Config.DevMgr.compressor.status == DeviceStatus::STATUS_OFF) {
		res = true;
	}
	else {
		res = Config.DevMgr.compressor.StopCompressor();
	}
	return res;
}

bool ScriptHeatPump::ForceStart() {
	return Start(true);
}