#include "Configuration.h"
#include "ScriptPump.h"
#include "ScriptHeatPump.h"
#include "Loger.h"

extern Configuration Config;

ScriptHeatPump::ScriptHeatPump(bool enable, String label, unsigned int alarmDelay) : Script(enable, label, alarmDelay)
{
	this->step = COMPRESSOR_IDLE;
}


ScriptHeatPump::~ScriptHeatPump()
{
}

bool ScriptHeatPump::IsAlarm() {
	bool res = false;
	res = !checkAllConditions();
	return res;
}


bool ScriptHeatPump::checkTempConditions() {
	bool ret = true;
	//Loger::Debug("Point 0");
	ret &= (Config.DevMgr.tGeoI->GetActionStatus() == ActionStatus::ACTION_NORMAL);
	//if (!ret) Loger::Debug("Point 1#"+String(Config.DevMgr.tGeoI->GetActionStatus()));
	ret &= (Config.DevMgr.tGeoO->GetActionStatus() == ActionStatus::ACTION_NORMAL);
	//ret &= (Config.DevMgr.tEvoI->GetActionStatus() == ActionStatus::ACTION_NORMAL);
	//ret &= (Config.DevMgr.tEvoO->GetActionStatus() == ActionStatus::ACTION_NORMAL);
	ret &= (Config.DevMgr.tCondI->GetActionStatus() == ActionStatus::ACTION_NORMAL);
	ret &= (Config.DevMgr.tCondO->GetActionStatus() == ActionStatus::ACTION_NORMAL);
	ret &= (Config.DevMgr.tHpO->GetActionStatus() == ActionStatus::ACTION_NORMAL);
	ret &= (Config.DevMgr.tHpI->GetActionStatus() == ActionStatus::ACTION_NORMAL);
	ret &= (Config.DevMgr.tCompr->GetActionStatus() == ActionStatus::ACTION_NORMAL);

	//Debug("Point 2#"+String(ret));

	//	Debug2("Check Internal Temp Conditions=", ret);
	return ret;
}
bool ScriptHeatPump::checkContactors() {
	bool ret = true;
	ret &= (Config.DevMgr.cFlow->GetActionStatus() == ActionStatus::ACTION_NORMAL);
	//Debug("CheckContactor=" + String(Config.DevMgr.cFlow->getActionStatus()));
	return ret;
}

bool ScriptHeatPump::IsStopNeeded(bool isSync) {
	bool res;

	res = (Config.GetControlTemperature() >= Config.OutTemp());
	return res;
}

bool ScriptHeatPump::IsStartNeeded(bool isSync) {
	bool res = false;
	res = (Config.GetControlTemperature() < Config.GetDesiredTemp());
	return res;
}


bool ScriptHeatPump::checkAllConditions() {
	bool res = true;
	res &= checkTempConditions();
	if (!res) Loger::Debug("ScriptHeatPump:Temp Condition Failed");
	res &= checkContactors();
	if (!res) Loger::Debug("ScriptHeatPump:Contactor Condition Failed");
	return res;
}

bool ScriptHeatPump::MainLoop(bool isSync) {

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
	
	
	switch (step) {

	case 0: { //Nothing to do
		step = COMPRESSOR_IS_START_NEEDED;
		Loger::Debug("ScriptHeatPump:The HeatPump starting");
		break;
	}
	case COMPRESSOR_IS_START_NEEDED: { // Is Start Needed

		if (IsStartNeeded(isSync)) {
			Loger::Debug("ScriptHeatPump:ScriptCompressor:Compressor: Start is needed");
			bool res1 = Config.ScriptMgr.scriptPumpContour1->Start(isSync);
			bool res2 = Config.ScriptMgr.scriptPumpContour2->Start(isSync);
			Loger::Debug("ScriptHeatPump:Pump contours:" + String(res1) + "#" + String(res2));
			if (res1 && res2) {
				step = COMPRESSOR_START_PUMP_GEO;
				Loger::Debug("ScriptHeatPump:Compressor: Geo pump starting!");
			}
		}
		else {
			Loger::Debug("ScriptHeatPump: Waiting for start allowance!");
		}
		break;
	}
	case COMPRESSOR_START_PUMP_GEO: { // Start Pump Geo
		if (IsStartNeeded(isSync)) {
			if (Config.ScriptMgr.scriptPumpGeo->Start(isSync)) {
				Loger::Debug("ScriptHeatPump:Compressor: Geo pump is done!" + String(Config.DevMgr.pumpGeo->status));
				step = COMPRESSOR_WAITING_PUMP_GEO_START;
				counterScript = Config.Counter1s;
			}
		}
		else {
			step = COMPRESSOR_IS_START_NEEDED;
		}
		break;
	}
	case COMPRESSOR_WAITING_PUMP_GEO_START: {//delay before start compressor
		if (Config.Counter1s - counterScript >= COMPRESSOR_ON_TIMEOUT) {
			step = COMPRESSOR_START;
			Loger::Debug("ScriptHeatPump:Geo Pump Started. Compressor starting...");
		}
		else {
			//Loger::Debug("ScriptHeatPump:Waiting Pump Geo#" + String(Config.Counter1s) + "#" + String(counterScript) );

		}
		break;
	}
	case COMPRESSOR_START: { //Start Compressor
		if (IsStartNeeded(isSync)) {
			if (StartCompressor(isSync)) {
				step = COMPRESSOR_IS_STOP_NEEDED;
				Loger::Debug("ScriptHeatPump:Compressor Started!");
			}
		}
		else {
			step = COMPRESSOR_STOP;
		}
		break;
	}
	case COMPRESSOR_IS_STOP_NEEDED: { // Is the temp achieved?
//		Debug("Compressor: is stop needed?");
		if (IsStopNeeded(isSync)) {
			step = COMPRESSOR_STOP;
			Loger::Debug("ScriptHeatPump:Compressor Stop!");
		}
		break;
	}
	case COMPRESSOR_STOP: { // Stop Compressor
		Loger::Debug("ScriptHeatPump:Compressor: Stop!");
		if (Config.DevMgr.compressor.StopCompressor()) {
			Loger::Debug("ScriptHeatPump:Compressor: Stop done!");
			step = COMPRESSOR_WAITING_PUMP_GEO_STOP;
			counterScript = Config.Counter1s;
			Loger::Debug("ScriptHeatPump:Compressor: Waiting before Geo Pump stop");
		}
		break;
	}
	case COMPRESSOR_WAITING_PUMP_GEO_STOP: {//delay before stop Pump Geo
		if (Config.Counter1s - counterScript >= PUMP_OFF_TIMEOUT) {
			step = COMPRESSOR_STOP_PUMP_GEO;
			Loger::Debug("ScriptHeatPump:Compressor:Stop Geo Pump ");
		}
		break;
	}
	case COMPRESSOR_STOP_PUMP_GEO: { // Stop Pump Geo
		if (Config.ScriptMgr.scriptPumpGeo->Stop(false)) {
			Loger::Debug("ScriptHeatPump:GeoPump Status=" + String(Config.DevMgr.pumpGeo->status));
			Loger::Debug("ScriptHeatPump:Compressor:Finish ");
			step = COMPRESSOR_IS_START_NEEDED;
			res = true;
		}
		break;
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
			counterScript = Config.Counter1s;
		}
	}
	if (step == COMPRESSOR_START
		|| step == COMPRESSOR_IS_STOP_NEEDED
		|| step == COMPRESSOR_STOP
		) { //Start Compressor
		if (StopCompressor(isSync)) {
			step = COMPRESSOR_STOP_PUMP_GEO;
			res = false;
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
			long waitingTime = Config.Counter1s - (Config.DevMgr.compressor.lastStatusTimestamp + Config.DevMgr.compressor.minTimeOff);
			if (waitingTime < 0) {//not ready to start
				if (isSync) {
					Loger::Debug("Start Compressor delay:" + String(-waitingTime));
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
					Loger::Debug("Critical conditions are achieved!");
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
		long waitingTime = Config.Counter1s - (Config.DevMgr.compressor.lastStatusTimestamp + Config.DevMgr.compressor.minTimeOn);

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
	res = Config.DevMgr.compressor.StopCompressor();
	res &= Config.ScriptMgr.scriptPumpGeo->ForceStop();
	return res;
}
