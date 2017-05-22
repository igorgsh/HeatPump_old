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
	
	
	Scenario* scriptPumpGeo;
	Scenario* scriptPumpContour1;
	Scenario* scriptPumpContour2;
	Scenario* scriptCompressor;
	
	Scenario* scripts[NUMBER_OF_SCRIPTS];

	void PrepareCmd(Scenario* script, ScenarioCmd cmd);
	void loop1(unsigned long counter);
	void loop5(unsigned long counter);
	void loop10(unsigned long counter);
	void begin();
private:
	DeviceManager* DevMgr;
	Scenario* currentScript=NULL;
	ScenarioCmd currentCmd = ScenarioCmd::SCENARIO_NOCMD;
	void init();
};

