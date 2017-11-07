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
/*
typedef enum {
	NO_ERROR = 0,
	SENSOR_DISCONNECTED = 1,
	LOW_VALUE = 2,
	HIGH_VALUE = 3,
	CONTACT_ERROR = 4
} ErrorCode;
*/
typedef enum {
	ACTION_NODATA = 0,
	ACTION_LOW_ALARM = 1,
	ACTION_LOW_ALARM_START = 2,
	ACTION_NORMAL = 3,
	ACTION_HIGH_ALARM_START = 4,
	ACTION_HIGH_ALARM = 5
} ActionStatus;

#define NUMBER_OF_ACTIONPOINTS 4

typedef enum {
	ACTIONPOINT_ALARM_LOW=0,
	ACTIONPOINT_START_LOW = 1,
	ACTIONPOINT_START_HIGH = 2,
	ACTIONPOINT_ALARM_HIGH = 3

}ActionPointType;

class Sensor
{
public:
	Sensor(String label, int pin, int critThreshold = 5);
	Sensor(String label, int pin, float actionPoints[], int critThreshold = 5);
	//Type of Sensor
	SensorType type = NOSENSOR;
	SensorType getType() { return type; };

	//Current value of sensor
	virtual float getValue() { return currentValue; };

	// Number of errors occured
	int ErrorCounter = 0;
	// Is error critical
	virtual bool isCritical() { return (ErrorCounter >= criticalThreshold); };

	//Label of Sensor
	String getLabel() { return label; };
	void setLabel(String lbl) { label = lbl; };

	//Pin where sensor is connected
	int getPin() { return pin; };

	// Request values from sensor
	virtual bool loop() = 0;

	bool getData();

	ActionStatus getActionStatus() { return actionStatus; };

	void setCriticalThreshold(int value) { criticalThreshold = value; };
	int getCriticalThreshold() { return criticalThreshold; };
	float getActionPoint(ActionPointType n) { if (n >= 0 && n < NUMBER_OF_ACTIONPOINTS) return actionPoints[n]; else return 0.0; };
	void setActionPoint(ActionPointType n, float value) { if (n >= 0 && n < NUMBER_OF_ACTIONPOINTS) actionPoints[n] = value; };

protected:
	// Is Data Ready for this Sensor
	virtual bool checkDataReady() = 0;
	//	ErrorCode error = NO_ERROR;
	String label;
	int pin;



	// how many times alarm should be set before it became critical
	int criticalThreshold;

	ActionStatus actionStatus = ActionStatus::ACTION_NODATA;

	float currentValue;

	float actionPoints[NUMBER_OF_ACTIONPOINTS] = { 0.0, 0.0, 0.0, 0.0 };
private:
	void init(String label, int pin, float actionPoints[], int critThreshold);

};

