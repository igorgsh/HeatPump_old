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
		actionStatus = checkStatus();

		if (actionStatus == ACTION_LOW_ALARM
			|| actionStatus == ACTION_HIGH_ALARM
			|| actionStatus == ACTION_ALARM) {
			ErrorCounter++;
		}
		else {
			ErrorCounter = 0;
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

