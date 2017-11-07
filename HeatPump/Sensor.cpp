#include "Sensor.h"


void Sensor::init(String label, int pin, float actionPoints[], int critThreshold) {

	this->label = label;
	this->pin = pin;
	for (int i = 0; i < NUMBER_OF_ACTIONPOINTS; i++) {
		if (actionPoints != NULL) {
			this->actionPoints[i] = actionPoints[i];
		}
	}
	this->criticalThreshold = critThreshold;
	pinMode(pin, INPUT);
}

Sensor::Sensor(String label, int pin, int critThreshold) {
	init(label, pin, NULL, critThreshold);
}


Sensor::Sensor(String label, int pin, float actionPoints[], int critThreshold) {
	init(label, pin, actionPoints, critThreshold);
}

bool Sensor::getData() {
	bool res;

	res = checkDataReady();
	if (res) { //data is ready
		if (currentValue <= actionPoints[ACTIONPOINT_ALARM_LOW]) {
			actionStatus = ACTION_LOW_ALARM;
			ErrorCounter++;
		}
		else if (currentValue <= actionPoints[ACTIONPOINT_START_LOW]) {
			actionStatus = ACTION_LOW_ALARM_START;
			ErrorCounter = 0;
		}
		else if (currentValue <= actionPoints[ACTIONPOINT_START_HIGH]) {
			actionStatus = ACTION_NORMAL;
			ErrorCounter = 0;
		}
		else if (currentValue <= actionPoints[ACTIONPOINT_ALARM_HIGH]) {
			actionStatus = ACTION_HIGH_ALARM_START;
			ErrorCounter = 0;
		}
		else if (currentValue > actionPoints[ACTIONPOINT_ALARM_LOW]) {
			actionStatus = ACTION_HIGH_ALARM;
			ErrorCounter++;
		}
	}
	else { //result is absent
		if (isCritical()) {
			actionStatus = ACTION_NODATA;
		}
		else {
			// Nothing todo. Keep the previous status
		}
	}
	return res;
}

