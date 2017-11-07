#pragma once
#include "Arduino.h"


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

	DeviceStatus status = STATUS_UNKNOWN;
	String getLabel() { return label; };
	void setLabel(String lbl) { label = lbl; };
	virtual void begin() = 0;
	unsigned long lastStatusTimestamp = 0;
	unsigned int minTimeOff = 0;
	unsigned int minTimeOn = 0;
protected:
	String label;
};

