#include "Sensor.h"


void Sensor::init(String label, int pin, float actionLow, float actionHigh, int critThreshold) {

	this->label = label;
	this->pin = pin;
	this->actionLow = actionLow;
	this->actionHigh = actionHigh;
	this->criticalThreshold = critThreshold;
}


Sensor::Sensor(String label, int pin,  float actionLow, float actionHigh, int critThreshold) {
	init(label, pin, actionLow, actionHigh, critThreshold);
}

bool Sensor::getData() {
	bool res;

	res = checkDataReady();
	if (res) { //data is ready
		if (actionLow <= currentValue) { // ActionLow
			actionStatus = ACTION_LOW;
			ErrorCounter++;
		}
		else if (actionHigh >= currentValue) { // ActionHigh
			actionStatus = ACTION_HIGH;
			ErrorCounter++;
		}
		else {
			actionStatus = ACTION_NORMAL;
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

