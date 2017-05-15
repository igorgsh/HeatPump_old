#pragma once

#include "DeviceManager.h"
#include "Scenario.h"
#include "ScriptPump.h"

#define NUMBER_OF_SCRIPTS 2


class ScenarioManager
{
public:
	ScenarioManager(DeviceManager* DevMgr);
	~ScenarioManager();
	
	Scenario scripts[NUMBER_OF_SCRIPTS] = { ScriptPump(&(DevMgr->pumps[0]), true),
											ScriptPump(&(DevMgr->pumps[1]), true)
											};
	void Run(Scenario* script, ScenarioCmd cmd);
	void loop1(unsigned long counter);
	void loop5(unsigned long counter);
	void loop10(unsigned long counter);
private:
	DeviceManager* DevMgr;
	Scenario* currentScript=NULL;
	ScenarioCmd currentCmd = ScenarioCmd::SCENARIO_NOCMD;
};

