#include "TempSensor.h"
//

TempSensor::TempSensor(String label, int pin,  float actionPoints[], int critThreshold)
	: Sensor(label, pin, actionPoints, critThreshold) {
	init();
}


TempSensor::~TempSensor()
{
	delete dt;
	delete wire;
}

void TempSensor::init() {
	this->type = THERMOMETER;
	wire = new OneWire(pin);
	dt = new DT(wire);
}

void TempSensor::requestTemperatures() {
	dt->requestTemperatures();
}

bool TempSensor::checkDataReady() {
	bool ret = dt->isConversionAvailable(0);
	if (ret) {
		currentValue = dt->getTempCByIndex(0);
	}
	return ret;
}

void TempSensor::begin() {
	dt->begin();
	dt->setResolution(DEFAULT_RESOLUTION);
}

bool TempSensor::loop(unsigned long counter) {
	bool result = true;
	if (counter % 10 == 0) {//first loop - request for data
		requestTemperatures();
		result = true;
	}
	else if (counter % 10 == 9) { // the last loop. All sensors which didn't ready are marked as Disconnected 
		if (!getData()) {
			actionStatus = ACTION_NODATA;
			ErrorCounter++;
			result = false;
		}
	}
	else { //check/prepare data ready
		result = getData();
	}
	return result;
}