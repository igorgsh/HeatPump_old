#include "TempSensor.h"
#include "Loger.h"
#include "Configuration.h"

extern Configuration Config;
extern Simulator* sim;


TempSensor::TempSensor(String label, int pin,  float lowerRange, float upperRange)
	: Sensor(label, pin, lowerRange, upperRange) {
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
	bool res = true;
	if (Config.IsSimulator()) {
	currentValue = sim->GetRealResult(this->pin);
	res = true;
	//Debug("Value for (" + String(this->getLabel()) + ") = " + String(currentValue));
	}
	else {
		res = dt->isConversionAvailable(0);
		if (res) {
			currentValue = dt->getTempCByIndex(0);
		}
	}
	if (res) {
		if (currentValue <= lowerRange
			|| currentValue >= upperRange) {
			actionStatus = ACTION_ALARM;
		}
		else {
			actionStatus = ActionStatus::ACTION_NORMAL;
		}
	}
	else {
		actionStatus = ActionStatus::ACTION_NODATA;
	}

	return res;
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
