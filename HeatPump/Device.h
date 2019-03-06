#pragma once
#include <Arduino.h>

typedef enum {
	NODEVICE = 0,
	THERMOMETER = 1,
	CONTACT = 2,
	PUMP = 3,
	COMPRESSOR = 4,
	RELAY = 5
} DeviceType;


class Device
{
public:
	Device(String lbl, DeviceType tp);
	~Device();
	// Type of Device
	DeviceType getType() { return type; };
	//void SetType(DeviceType deviceType) { type = deviceType; }
	//Label of Sensor
	String getLabel() { return label; };
	void setLabel(String lbl) { label = lbl; };
private:
	DeviceType type = DeviceType::NODEVICE;
	String label;


};

