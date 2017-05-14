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
	
	Scenario scripts[NUMBER_OF_SCRIPTS] = { ScriptPump(&(DevMgr->pumps[0])),
											ScriptPump(&(DevMgr->pumps[1]))
											};
	
	void loop5(unsigned long counter);
private:
	DeviceManager* DevMgr;
};

