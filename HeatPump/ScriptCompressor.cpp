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

bool ScriptCompressor::checkTempInt() {
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
	Debug2("checkTempInt=", ret);
	return ret;
}
bool ScriptCompressor::checkContactors() {
	bool ret = true;
	ret &= (Config.DevMgr.cFlow->getActionStatus() == ActionStatus::ACTION_NORMAL);
	Debug2("CheckContactor=", ret);
	return ret;
}

bool ScriptCompressor::IsStartNeeded() {
	//if (Config.getDesiredTemp() < Config.DevMgr.currentTemp->getValue())
	bool res;

	res = (Config.DevMgr.tHpO->getValue() < Config.OutTemperature());
	Debug2("IsStartNeeded=", res);
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
			&& checkTempInt()
			&& checkContactors()) {
			//Debug("Normal");
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

	Debug("Compressor Start");
	Debug2("Step:", String(step));
	bool result = false;
	if (step == 0) {
		counterScript = Config.counter1s;
		result = Config.ScenMgr.scriptPumpGeo->Start();
		if (result) {
			step=1;
		}
	} 
	if (step == 1) {
		result = Config.ScenMgr.scriptPumpContour1->Start();
		if (result) {
			step = 2;
		}
	}
	if (step == 2) {
		result = Config.ScenMgr.scriptPumpContour2->Start();
		if (result) {
			counterScript = Config.counter1s;
			step = 3;
		}
	}
	if (step ==3 ) {//delay before start compressor
		Debug("Wait:" + String(Config.counter1s - counterScript));
		if (Config.counter1s - counterScript >= COMPRESSOR_ON_TIMEOUT) {
			step = 4;
		}
	}
	if (step == 4) { //final
		if (comp->lastStatusTimestamp + comp->minTimeOff <= Config.counter1s) { //device is off a long time
			result = comp->StartCompressor();
		}
		else {
			result = false;
		}
	}

	Debug2("Return:", String(result));
	return result;
}

bool ScriptCompressor::Stop() {
	Debug("Compressor Stop");
	Debug2("Step:", String(step));
	bool result = false;
	if (step == 0) {
		if (comp->lastStatusTimestamp + comp->minTimeOn <= Config.counter1s) { //device is on a long time
			result = comp->StopCompressor();
		}
		step = 1;
		counterScript = Config.counter1s;
	}
	if (step == 1) {
		if (Config.counter1s - counterScript >= PUMP_OFF_TIMEOUT) {
			step = 2;
		}
	}
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
	}
	if (step == 4) { //final
		result = Config.ScenMgr.scriptPumpGeo->Stop();
	}

	return result;
}

bool ScriptCompressor::IsForceOff() {
	return false;
}