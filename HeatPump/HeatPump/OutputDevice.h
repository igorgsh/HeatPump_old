#pragma once
#include "Arduino.h"
//#include "ScenarioItem.h"


typedef enum {
	STATUS_UNKNOWN = 0,
	STATUS_ON = 1,
	STATUS_OFF = 2
} DeviceStatus;

class OutputDevice
{
public:
	OutputDevice();
	~OutputDevice();

//	void Command(ScenarioCommand cmd);
//	virtual bool DeviceCommand(ScenarioCommand cmd);
//	bool IsDeviceReady() { return (nextDeviceReady <= Config.counter1); };
//	ScenarioCommand currentCmd = ScenarioCommand::COMMAND_NO_COMMAND;
	//void(*callBack)() = NULL;
	DeviceStatus status = STATUS_UNKNOWN;
	String getLabel() { return label; };
	void setLabel(String lbl) { label = lbl; };
	virtual void begin() = 0;
	unsigned long lastStatusTimestamp = 0;
	unsigned int minTimeOff = 0;
	unsigned int minTimeOn = 0;
protected:
	String label;
	//int pin;
	//void WaitTime(unsigned long delay, void (*callBack)());
//	void WaitTime(unsigned long delay);
//	unsigned long nextDeviceReady = 0;
	//void SetDelay(long time, )
};

