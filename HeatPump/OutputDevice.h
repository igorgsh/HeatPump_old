#pragma once
#include "Arduino.h"
#include "Device.h"

typedef enum {
	STATUS_UNKNOWN = 0,
	STATUS_ON = 1,
	STATUS_OFF = 2,
	STATUS_ERROR = 3,
	STATUS_IDLE = 4
} DeviceStatus;

class OutputDevice : 
	public Device
{
public:
	OutputDevice(String lbl, DeviceType tp);
	~OutputDevice();

	DeviceStatus status = STATUS_UNKNOWN;
	virtual void begin() = 0;
	unsigned long lastStatusTimestamp = 0;
	unsigned int minTimeOff = 0;
	unsigned int minTimeOn = 0;
protected:
	String label;
};

