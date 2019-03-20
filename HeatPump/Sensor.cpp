#include "Sensor.h"

void Sensor::init(int pin, float lowerRange, float upperRange) {

	this->pin = pin;
	this->lowerRange = lowerRange;
	this->upperRange = upperRange;
	pinMode(pin, INPUT);
}

Sensor::Sensor(UnitType sensorType) : Unit("", sensorType){
	init(-1, 0.0, 0.0);
}

Sensor::Sensor(String label, int pin, UnitType sensorType, float lowerRange, float upperRange) : Unit(label, sensorType) {
	init(pin, lowerRange, upperRange);
}

/*
bool Sensor::GetData() {
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
*/

