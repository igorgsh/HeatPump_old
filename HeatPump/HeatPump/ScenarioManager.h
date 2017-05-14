#pragma once
typedef enum {
	SCENARIO_NOCMD = 0,
	SCENARIO_STOP = 1,
	SCENARIO_START =2 
} ScenarioCmd;

class ScenarioManager
{
public:
	ScenarioManager();
	~ScenarioManager();
	void loop5(unsigned long counter);
};

