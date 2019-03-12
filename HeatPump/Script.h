#pragma once
#include "Arduino.h"
#include "OutputDevice.h"

class Script
{
public:
	Script(bool enabled, String label, unsigned int alarmDelay/*, OutputDevice* dev*/);
	bool IsActive = false;
	int getId() { return id; };
	String getLabel() { return label; };
	void setLabel(String lbl) { label = lbl; };
	bool Enabled = true;
	unsigned long GetAlarm() { return alarm; }
	void SetAlarm(unsigned long ts) { alarm = ts; }
	int GetStep() { return step; }
	void SetStep(int st) { step = st; }
	unsigned long GetAlarmDelay() {	return alarmDelay;}

	virtual ~Script();
	virtual bool MainLoop(bool isSync) = 0;
	virtual bool IsAlarm() = 0;
	virtual bool Start(bool isSync) = 0;
	virtual bool Stop(bool isSync) = 0;
	virtual bool ForceStop() = 0;

private:
	int id;
	String label;
	void generateId();
	void generateLabel();

protected:
	int step = 0;
	unsigned long counterScript = 0;
	unsigned long alarm = 0;
	unsigned int alarmDelay;
};

