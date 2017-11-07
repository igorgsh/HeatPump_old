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
	if (IsStartNeeded()
		&& checkTempInt()
		&& checkContactors()) {
		//Debug("Normal");
		if (comp->status == STATUS_ON) {
			ret = SCENARIO_NOCMD;
		}
		else {
			ret = ScenarioCmd::SCENARIO_START;
		}
	}
	else {
		//Debug("Not Normal");

		if (comp->status == STATUS_OFF) {
			ret = SCENARIO_NOCMD;
		}
		else {
			ret = SCENARIO_STOP;
		}
	}
	return ret;
}

bool ScriptCompressor::Start() {

	Debug("Compressor Start");
	Debug2("Step:", String(step));
	bool ret = false;
	bool result = false;
	if (step == 0) {
		counterScript = Config.counter1s;
		ret = Config.ScenMgr.scriptPumpGeo->Start();
		if (ret) {
			step=1;
		}
	} 
	if (step == 1) {
		ret = Config.ScenMgr.scriptPumpContour1->Start();
		if (ret) {
			step = 2;
		}
	}
	if (step == 2) {
		ret = Config.ScenMgr.scriptPumpContour2->Start();
		if (ret) {
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
		comp->StartCompressor();
		result = true;
		step = 0;
	}

	Debug2("Return:", String(result));
	return result;
}

bool ScriptCompressor::Stop() {
	Debug("Compressor Stop");
	Debug2("Step:", String(step));
	bool result = false;
	bool ret = false;
	if (step == 0) {
		comp->StopCompressor();
		ret = true;
		step = 1;
		counterScript = Config.counter1s;
	}
	if (step == 1) {
		if (Config.counter1s - counterScript >= PUMP_OFF_TIMEOUT) {
			step = 2;
		}
	}
	if (step == 2) {
		ret = Config.ScenMgr.scriptPumpContour2->Stop();
		if (ret) {
			step = 3;
		}
	}
	if (step == 3) {
		ret = Config.ScenMgr.scriptPumpContour1->Stop();
		if (ret) {
			step = 4;
		}
	}
	if (step == 4) { //final
		ret = Config.ScenMgr.scriptPumpGeo->Stop();
		step = 0;
		result = true;
	}

	return result;
}