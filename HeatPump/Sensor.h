#pragma once
#include "Arduino.h"

#include "Device.h"
#include "Simulator.h"



typedef enum {
	ACTION_NODATA = 0,
	ACTION_NORMAL = 1,
	ACTION_ALARM = 2
} ActionStatus;

class Sensor : 
	public Device
{
public:
	Sensor(String lbl, DeviceType tp, int pin);
	Sensor(String lbl, DeviceType tp, int pin, float lowerRange, float higherRange);
	//Type of Sensor

	//Current value of sensor
	virtual float getValue() { return currentValue; };

	// Number of errors occured
	int ErrorCounter = 0;
	int ErrorNoData = 0;


	//Pin where sensor is connected
	int getPin() { return pin; };

	// Request values from sensor
	virtual bool loop() = 0;

	bool getData();

	ActionStatus getActionStatus() { 
		if (actionStatus == ActionStatus::ACTION_NODATA) {
			getData();
		}
		return actionStatus; };
	float GetLowerRange() { return lowerRange; }
	float GetUpperRange() { return upperRange; }
protected:
	// Is Data Ready for this Sensor
	virtual bool checkDataReady() = 0;
//	virtual ActionStatus checkStatus() = 0;
	int pin;

	ActionStatus actionStatus = ActionStatus::ACTION_NODATA;

	float currentValue;
	float lowerRange;
	float upperRange;
private:
	void init(int pin, float lowerRange, float upperRange);

};

