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
	virtual ~Scenario();
	bool IsActive = false;
	virtual ScenarioCmd TriggerredCmd() = 0 ;
	virtual bool Run(ScenarioCmd cmd, unsigned long counter);
	int getId() { return id; };
	String getLabel() { return label; };
	void setLabel(String lbl) { label = lbl; };
	bool Enabled = true;
	//virtual bool Run(ScenarioCmd cmd, unsigned long counter) =0;
	virtual bool Start()=0;
	virtual bool Stop()=0;
	virtual bool begin()=0;
private:
	//ScenarioManager* scenMgr;
	int id;
	String label;
	
	void generateId();
	void generateLabel();
	OutputDevice* device;
};

