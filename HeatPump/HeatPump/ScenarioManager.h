#pragma once

#include "DeviceManager.h"
#include "Scenario.h"
#include "ScriptPump.h"
#include "ScriptCompressor.h"

#define NUMBER_OF_SCRIPTS 4



class ScenarioManager
{
public:
	ScenarioManager(DeviceManager* DevMgr);
	~ScenarioManager();
	
	Scenario scripts[NUMBER_OF_SCRIPTS] = { ScriptPump(DevMgr->pumpGeo, true),
											ScriptPump(DevMgr->pumpContour1, true),
											ScriptPump(DevMgr->pumpContour2, true),
											ScriptCompressor(&(DevMgr->compressor), true, scriptPumpGeo, scriptPumpContour1, scriptPumpContour2)

										};
	Scenario* scriptPumpGeo = &scripts[0];
	Scenario* scriptPumpContour1 = &scripts[1];
	Scenario* scriptPumpContour2 = &scripts[2];
	Scenario* scriptCompressor = &scripts[3];

	void PrepareCmd(Scenario* script, ScenarioCmd cmd);
	void loop1(unsigned long counter);
	void loop5(unsigned long counter);
	void loop10(unsigned long counter);
private:
	DeviceManager* DevMgr;
	Scenario* currentScript=NULL;
	ScenarioCmd currentCmd = ScenarioCmd::SCENARIO_NOCMD;
};

