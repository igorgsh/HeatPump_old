#include "Sensor.h"

/*
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
*/
void Sensor::init(String label, int pin, float lowerRange, float upperRange) {

	this->label = label;
	this->pin = pin;
	this->lowerRange = lowerRange;
	this->upperRange = upperRange;
	pinMode(pin, INPUT);
}

Sensor::Sensor(String label, int pin) {
	init(label, pin, 0, 0);
}



Sensor::Sensor(String label, int pin, float lowerRange, float upperRange) {
	init(label, pin, lowerRange, upperRange);
}


bool Sensor::getData() {
	bool res;

	res = checkDataReady();
	if (res) { //data is ready
		actionStatus = checkStatus();
		ErrorNoData = 0;
		if (actionStatus == ActionStatus::ACTION_ALARM) {
			ErrorCounter++;
		}
		else {
			ErrorCounter = 0;
		}
	}
	else { //result is absent
		actionStatus = ACTION_NODATA;
		ErrorNoData++;
	}
	return res;
}

