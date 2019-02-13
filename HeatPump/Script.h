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
class Script
{
public:
	Script(bool enabled, String label, OutputDevice* dev);
	virtual ~Script();
	bool IsActive = false;
	bool Run(ScenarioCmd cmd);
	int getId() { return id; };
	String getLabel() { return label; };
	void setLabel(String lbl) { label = lbl; };
	bool Enabled = true;
	virtual bool Start(bool isSync)=0;
	virtual bool Stop(bool isSync)=0;
	//virtual bool ForceStop() = 0;
	virtual bool begin()=0;
	int step = 0;
	bool IsCompleted() { return (lastCmd==SCENARIO_NOCMD && step==0); }
private:
	int id;
	String label;
	//bool completed = true;
	void generateId();
	void generateLabel();
	OutputDevice* device;
	//bool RunCmd();

protected:
	unsigned long counterScript = 0;
	//bool(*lastCommand)(void);
	//ScenarioCmd prevCmd = ScenarioCmd::SCENARIO_NOCMD;
	ScenarioCmd lastCmd = ScenarioCmd::SCENARIO_NOCMD;
	//virtual ScenarioCmd TriggerredCmd() = 0;
	
};

