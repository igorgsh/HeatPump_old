#pragma once
#include "Definitions.h"
//#include "ScenarioManager.h"

class Scenario
{
public:
	Scenario();
	~Scenario();
	bool IsActive = false;
	virtual ScenarioCmd TriggerredCmd() { return ScenarioCmd::SCENARIO_NOCMD; };
private:
	//ScenarioManager* scenMgr;
};

