#pragma once
#include "Arduino.h"
#include "Definitions.h"
#include "OutputDevice.h"

class Script
{
public:
	Script(bool enabled, String label, unsigned int alarmDelay/*, OutputDevice* dev*/);
	virtual ~Script();
	bool IsActive = false;
	bool Run(ScenarioCmd cmd);
	int getId() { return id; };
	String getLabel() { return label; };
	void setLabel(String lbl) { label = lbl; };
	bool Enabled = true;
	virtual bool Start(bool isSync)=0;
	virtual bool Stop(bool isSync)=0;
	virtual bool ForceStop() = 0;
	virtual bool ForceStart() = 0;
//	virtual bool begin()=0;
	int step = 0;
	bool IsCompleted() { return (lastCmd==SCENARIO_NOCMD && step==0); }
	bool IsStartAlarm() { return startAlarm > 0; }
	bool IsStopAlarm() { return stopAlarm > 0; }

private:
	int id;
	String label;
	void generateId();
	void generateLabel();
	//OutputDevice* device;

protected:
	unsigned long counterScript = 0;
	ScenarioCmd lastCmd = ScenarioCmd::SCENARIO_NOCMD;
	unsigned long startAlarm = 0;
	unsigned long stopAlarm = 0;
	virtual void CheckStartAlarm(bool isSync) = 0;
	virtual void CheckStopAlarm(bool isSync) = 0;
	unsigned int alarmDelay;
};

