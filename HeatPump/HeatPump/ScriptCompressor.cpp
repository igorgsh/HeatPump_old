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
	return ret;
}
bool ScriptCompressor::checkContactors() {
	bool ret = true;
	ret &= (Config.DevMgr.cFlow->getActionStatus() == ActionStatus::ACTION_NORMAL);
	return ret;
}

bool ScriptCompressor::IsStartNeeded() {
	if (Config.getDesiredTemp() < Config.DevMgr.currentTemp->getValue())
		return false;
	else
		return true;
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
	static int step = 0;

	Debug("Compressor Start");
	Debug2("Step:", step);
	bool ret = false;
	if (step == 0) {
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
			step = 3;
		}
	}
	if (step == 3) { //final
		comp->StartCompressor();
		ret = true;
		step = 0;
	}

	Debug2("Return:", ret);
	return ret;
}

bool ScriptCompressor::Stop() {
	static int step = 0;
	bool ret = false;
	if (step == 0) {
		ret = Config.ScenMgr.scriptPumpGeo->Stop();
		if (ret) {
			step = 1;
		}
	}
	if (step == 1) {
		ret = Config.ScenMgr.scriptPumpContour1->Stop();
		if (ret) {
			step = 2;
		}
	}
	if (step == 2) {
		ret = Config.ScenMgr.scriptPumpContour2->Stop();
		if (ret) {
			step = 3;
		}
	}
	if (step == 3) { //final
		comp->StopCompressor();
		ret = true;
		step = 0;
	}

	return ret;
}