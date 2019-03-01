#pragma once
#include "Arduino.h"

#ifdef _SIMULATOR_
#include "Simulator.h"
#endif // _SIMULATOR_


typedef enum {
	NOSENSOR = 0,
	THERMOMETER = 1,
	CONTACT = 2
} SensorType;


typedef enum {
	ACTION_NODATA = 0,
	ACTION_NORMAL = 1,
	ACTION_ALARM = 2
} ActionStatus;

class Sensor
{
public:
	Sensor(String label, int pin);
	Sensor(String label, int pin, float lowerRange, float higherRange);
	//Type of Sensor
	SensorType type = NOSENSOR;
	SensorType getType() { return type; };

	//Current value of sensor
	virtual float getValue() { return currentValue; };

	// Number of errors occured
	int ErrorCounter = 0;
	int ErrorNoData = 0;

	//Label of Sensor
	String getLabel() { return label; };
	void setLabel(String lbl) { label = lbl; };

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
	String label;
	int pin;




	ActionStatus actionStatus = ActionStatus::ACTION_NODATA;

	float currentValue;
	float lowerRange;
	float upperRange;
private:
	void init(String label, int pin, float lowerRange, float upperRange);

};

