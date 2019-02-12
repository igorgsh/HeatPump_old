#include "ScriptCompressor.h"
#include "Configuration.h"

extern Configuration Config;

ScriptCompressor::ScriptCompressor(Compressor* compressor, bool enable, String label, Scenario* pGeo, Scenario* p1, Scenario* p2) : Scenario(enable, label, compressor)
{
	this->comp = compressor;
	this->pumpGeo = (ScriptPump*)pGeo;
	this->pumpContour1 = (ScriptPump*)p1;
	this->pumpContour2 = (ScriptPump*)p2;
}


ScriptCompressor::~ScriptCompressor()
{
}

bool ScriptCompressor::checkInternalTempConditions() {
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
bool ScriptCompressor::checkContactors() {
	bool ret = true;
	ret &= (Config.DevMgr.cFlow->getActionStatus() == ActionStatus::ACTION_NORMAL);
//	Debug2("CheckContactor=", ret);
	return ret;
}

bool ScriptCompressor::IsStartNeeded() {
	bool res;

	res = (Config.ControlTemperature() < Config.OutTemperature());
//	Debug2("IsStartNeeded=", res);
	return res;
}

ScenarioCmd ScriptCompressor::TriggerredCmd() {

	ScenarioCmd ret = SCENARIO_NOCMD;
	//Debug("ScriptCompressor.Trigger");
	// check all internal temp sensors
	if (IsForceOff()) {
		ret = ScenarioCmd::SCENARIO_STOP;
	}
	else {
		if (IsStartNeeded()
			&& checkInternalTempConditions()
			&& checkContactors()) {
			//Debug("Compressor start ready");

			if (comp->status == STATUS_ON) {
				ret = ScenarioCmd::SCENARIO_NOCMD;
			}
			else {
				ret = ScenarioCmd::SCENARIO_START;
			}
		}
		else {
			//Debug("Not Normal");
			if (comp->status == STATUS_OFF) {
				ret = ScenarioCmd::SCENARIO_NOCMD;
			}
			else {
				ret = ScenarioCmd::SCENARIO_STOP;
			}
		}

	}
	return ret;
}

bool ScriptCompressor::Start() {

	//Debug("Compressor Start:" + String(step));
	bool res = false;
	if (step == 0) {
		counterScript = Config.counter1s;
		if (Config.ScenMgr.scriptPumpGeo->Start()) {
			step=1;
		}
	} 
	if (step == 1) {
		if (Config.ScenMgr.scriptPumpContour1->Start()) {
			step = 2;
		}
	}
	if (step == 2) {
		if (Config.ScenMgr.scriptPumpContour2->Start()) {
			counterScript = Config.counter1s;
			step = 3;
		}
	}
	if (step ==3 ) {//delay before start compressor
		Debug("Wait:" + String(COMPRESSOR_ON_TIMEOUT - (Config.counter1s - counterScript)));
		if (Config.counter1s - counterScript >= COMPRESSOR_ON_TIMEOUT) {
			step = 4;
		}
	}
	if (step == 4) { //final
		if (comp->lastStatusTimestamp + comp->minTimeOff <= Config.counter1s) { //device is off a long time
			res = comp->StartCompressor();
			Debug("Compressor Started = " + String(res));
		}
	}

	//Debug2("Return:", String(res));
	return res;
}

bool ScriptCompressor::Stop() {
	//Debug("Compressor Stop:" + String(step));
	bool result = false;
	if (step == 0) {
		if (comp->lastStatusTimestamp + comp->minTimeOn <= Config.counter1s) { //device is on a long time
			result = comp->StopCompressor();
			Debug("Compressor stopped = " + String(result));
		}
		step = 1;
		counterScript = Config.counter1s;
	}
	if (step == 1) {
		if (Config.counter1s - counterScript >= PUMP_OFF_TIMEOUT) {
			step = 2;
		}
	}/*
	if (step == 2) {
		result = Config.ScenMgr.scriptPumpContour2->Stop();
		if (result) {
			step = 3;
		}
	}
	if (step == 3) {
		result = Config.ScenMgr.scriptPumpContour1->Stop();
		if (result) {
			step = 4;
		}
	}*/
	if (step == 2) { //final
		result = Config.ScenMgr.scriptPumpGeo->Stop();
	}

	return result;
}

bool ScriptCompressor::IsForceOff() {
	return false;
}