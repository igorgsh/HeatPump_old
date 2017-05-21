#pragma once
#include "Arduino.h"
#include "Definitions.h"
#include "OutputDevice.h"

//#include "ScenarioManager.h"
/*
typedef enum {
	SCENARIO_NOT_RUN=0,
	SCENARIO_RUN = 1
} ScenarioStatus;
*/
class Scenario
{
public:
	Scenario(bool enabled, String label, OutputDevice* dev);
	~Scenario();
	bool IsActive = false;
	virtual ScenarioCmd TriggerredCmd() { return ScenarioCmd::SCENARIO_NOCMD; };
	//virtual bool Run(ScenarioCmd cmd, unsigned long counter) { return false; };
	int getId() { return id; };
	String getLabel() { return label; };
	void setLabel(String lbl) { label = lbl; };
	bool Enabled = true;
	virtual bool Run(ScenarioCmd cmd, unsigned long counter);
	virtual bool Start() { return true; };
	virtual bool Stop() { return true;};

private:
	//ScenarioManager* scenMgr;
	int id;
	String label;
	
	void generateId();
	void generateLabel();
	OutputDevice* device;
};

