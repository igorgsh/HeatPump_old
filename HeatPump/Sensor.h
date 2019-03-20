#pragma once
#include "Arduino.h"

#include "Simulator.h"
#include "Unit.h"


typedef enum {
	ACTION_NODATA = 0,
	ACTION_NORMAL = 1,
	ACTION_ALARM = 2
} ActionStatus;

class Sensor : public Unit
{
public:
	Sensor(String label, int pin, UnitType sensorType, float lowerRange=0.0, float higherRange=0.0);
	Sensor(UnitType sensorType);

	//Current value of sensor
	virtual float GetValue() { return currentValue; };

	// Number of errors occured
	int ErrorCounter = 0;
	int ErrorNoData = 0;


	//Pin where sensor is connected
	int GetPin() { return pin; };

	// Request values from sensor
	virtual bool loop() = 0;

	bool GetData();

	ActionStatus GetActionStatus() { 
		if (actionStatus == ActionStatus::ACTION_NODATA) {
			GetData();
		}
		return actionStatus; };
	float GetLowerRange() { return lowerRange; }
	float GetUpperRange() { return upperRange; }

protected:
	virtual bool checkDataReady() = 0;
	int pin;

	ActionStatus actionStatus = ActionStatus::ACTION_NODATA;

	float currentValue;
	float lowerRange;
	float upperRange;
private:
	void init(int pin, float lowerRange, float upperRange);
};

