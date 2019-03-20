#include "TempSensor.h"
#include "Configuration.h"

extern Configuration Config;
extern Simulator* sim;



TempSensor::TempSensor(String label, float lowerRange, float upperRange) : Sensor(label, -1, UnitType::UT_Thermometer, lowerRange, upperRange)
{
	
	for (int i = 0; i < 8; i++) {
		DevAddress[i] = 0;
	}
	DevAvailable = false;
}

TempSensor::~TempSensor()
{
}

bool TempSensor::loop() {
	bool ret = false;
	float oldValue = currentValue;

	if (Config.IsSimulator()) {
		currentValue = sim->GetRealResult(this->pin);
		ret = true;
		//Debug("Value for (" + String(this->getLabel()) + ") = " + String(currentValue));
	}
	else {
		if (DevAvailable) {
			currentValue = Driver->GetTemperature(DevAddress);
			ret = true;
		}
		else {
			actionStatus = ActionStatus::ACTION_NODATA;
			currentValue = TEMPERATURE_FAILED;
			ret = false;
		}
	}
	if (ret) {
		if (currentValue <= lowerRange
			|| currentValue >= upperRange) {
			actionStatus = ACTION_ALARM;
		}
		else {
			actionStatus = ActionStatus::ACTION_NORMAL;
		}
	}

	if (ret && oldValue != currentValue) {
		Config.GetMqttClient()->Publish(this);
	}
	return ret;
}

void TempSensor::begin() {
}
