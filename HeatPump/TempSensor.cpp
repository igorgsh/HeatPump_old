#include "TempSensor.h"
#include "Definitions.h"
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
	bool ret = true;
#ifdef _SIMULATOR_
	currentValue = sim->GetRealResult(this->pin);
	//Debug("Value for (" + String(this->getLabel()) + ") = " + String(currentValue));
#else
	ret = dt->isConversionAvailable(0);
	if (ret) {
		currentValue = dt->getTempCByIndex(0);
	}
#endif // _SIMULATOR_
	return ret;
}

void TempSensor::begin() {
	dt->begin();
	dt->setResolution(DEFAULT_RESOLUTION);
}

bool TempSensor::loop() {
	bool result = true;

	if (tryCounter == 0) {//first loop - request for data
		requestTemperatures();
		result = true;
		tryCounter++;
	}
	else {
		result = getData();
		if (result) {
			tryCounter = 0;
		}
		else if (tryCounter == 9) {
			actionStatus = ACTION_NODATA;
			ErrorCounter++;
			tryCounter = 0;
		}
		else {
			tryCounter++;
		}
	}
	//Debug("temp=" + this->getLabel() + ";TryCounter=" + String(tryCounter)+";Temp="+String(currentValue));
	return result;
}

ActionStatus TempSensor::checkStatus() {
	ActionStatus status = ActionStatus::ACTION_NORMAL;

	if (currentValue <= actionPoints[ACTIONPOINT_ALARM_LOW]) {
		status = ACTION_LOW_ALARM;
		ErrorCounter++;
	}
	else if (currentValue <= actionPoints[ACTIONPOINT_START_LOW]) {
		status = ACTION_LOW_ALARM_START;
		ErrorCounter = 0;
	}
	else if (currentValue <= actionPoints[ACTIONPOINT_START_HIGH]) {
		status = ACTION_NORMAL;
		ErrorCounter = 0;
	}
	else if (currentValue <= actionPoints[ACTIONPOINT_ALARM_HIGH]) {
		status = ACTION_HIGH_ALARM_START;
		ErrorCounter = 0;
	}
	else if (currentValue > actionPoints[ACTIONPOINT_ALARM_LOW]) {
		status = ACTION_HIGH_ALARM;
		ErrorCounter++;
	}

	return status;
}