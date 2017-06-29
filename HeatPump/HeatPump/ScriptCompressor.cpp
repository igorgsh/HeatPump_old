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

ScenarioCmd ScriptCompressor::TriggerredCmd() {

	ScenarioCmd ret = SCENARIO_NOCMD;
	//Debug("ScriptCompressor.Trigger");
	// check TGEO & T1
	if (Config.DevMgr.t1->getActionStatus() == ActionStatus::ACTION_NORMAL
		&& Config.DevMgr.tGeo->getActionStatus() == ActionStatus::ACTION_NORMAL) {
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