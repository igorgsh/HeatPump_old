#pragma once

#include "DeviceManager.h"
#include "Script.h"
#include "ScriptPump.h"
#include "ScriptCompressor.h"

#define NUMBER_OF_SCRIPTS	4
//#define HANGOUT_INTERVAL	(10*60)


class ScriptManager
{
public:
	ScriptManager(DeviceManager* DevMgr);
	~ScriptManager();
/*	
	Script* scripts[NUMBER_OF_SCRIPTS] = {
		ScriptPump(DevMgr->pumpContour1, true, "P1", PumpMode::ALWAYS_START),
		ScriptPump(DevMgr->pumpContour2, true, "P2", PumpMode::ALWAYS_START),
		ScriptCompressor(&(DevMgr->compressor), true,"C1", scriptPumpGeo, scriptPumpContour1, scriptPumpContour2)
	};
*/

	Script* scriptPumpGeo;
	Script *scriptPumpContour1;
	Script *scriptPumpContour2;
	Script* scriptCompressor;
//	void PrepareCmd(Scenario* script, ScenarioCmd cmd);
	void loop();
	bool setup();
	void SetCmd(ScenarioCmd cmd);
private:
	DeviceManager* DevMgr;
	//Scenario* currentScript=NULL;
	//ScenarioCmd currentCmd = ScenarioCmd::SCENARIO_NOCMD;
	void init();
	ScenarioCmd lastCmd;
};

