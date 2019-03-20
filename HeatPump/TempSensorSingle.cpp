#include "TempSensorSingle.h"
#include "Loger.h"
#include "Configuration.h"

extern Configuration Config;
extern Simulator* sim;


TempSensorSingle::TempSensorSingle(String label, int pin,  float lowerRange, float upperRange)
	: Sensor(label, pin, UnitType::UT_Thermometer, lowerRange, upperRange) {
	init();
}


TempSensorSingle::~TempSensorSingle()
{
	delete dt;
	delete wire;
}

void TempSensorSingle::init() {
	wire = new OneWire(pin);
//	dt = new DT(wire);
	dt = new DallasTemperature(wire);
}

void TempSensorSingle::requestTemperatures() {
	dt->requestTemperatures();
}

bool TempSensorSingle::checkDataReady() {
	bool res = true;
	float oldValue = currentValue;
	if (Config.IsSimulator()) {
		currentValue = sim->GetRealResult(this->pin);
		res = true;
		//Debug("Value for (" + String(this->getLabel()) + ") = " + String(currentValue));
	}
	else {
		//res = dt->isConversionAvailable(0);
		res = dt->isConversionComplete();
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
	if (oldValue != currentValue) {
		Config.GetMqttClient()->Publish(this);
	}
	return res;
}

void TempSensorSingle::begin() {
	dt->begin();
	dt->setResolution(DT_DEFAULT_RESOLUTION);
	
}

bool TempSensorSingle::loop() {
	bool result = true;

	if (tryCounter == 0) {//first loop - request for data
		requestTemperatures();
		result = true;
		tryCounter++;
	}
	else {
		result = GetData();
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
