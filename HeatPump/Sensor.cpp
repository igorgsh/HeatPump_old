#include "Sensor.h"

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
	//Debug("GetData:" + String(res));
	if (res) { //data is ready
//		actionStatus = checkStatus();
		ErrorNoData = 0;
		if (actionStatus == ActionStatus::ACTION_ALARM) {
			//Debug("ALARM:" + getLabel());
		
			ErrorCounter++;
		}
		else {
			actionStatus = ActionStatus::ACTION_NORMAL;
			//Debug("NOT ALARM");
			ErrorCounter = 0;
		}
	}
	else { //result is absent
		actionStatus = ACTION_NODATA;
		//Debug("NO DATA");
		ErrorNoData++;
	}
	return res;
}

