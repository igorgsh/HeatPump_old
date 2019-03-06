#include "Contactor.h"
#include "Definitions.h"

extern Configuration Config;
extern Simulator* sim;


Contactor::~Contactor()
{
}


Contactor::Contactor(String label, int pin, bool lhOn, bool alarmOn)
	: Sensor(label,DeviceType::CONTACT, pin) {
	this->lhOn = lhOn;
	this->AlarmOn = alarmOn;
	this->lowerRange = lhOn;
	this->upperRange = lhOn;
	init();

}



void Contactor::init() {
}

ActionStatus Contactor::checkStatus() {
	return (currentValue == AlarmOn? ActionStatus::ACTION_ALARM : ActionStatus::ACTION_NORMAL);
}

void Contactor::begin() {
}

bool Contactor::loop() {
	bool res;

	res = getData();
	return res;
}

bool Contactor::checkDataReady() {

	float oldValue = currentValue;

	if (Config.IsSimulator()) {
		currentValue = sim->GetRealResult(pin);
	}
	else {
		currentValue = digitalRead(pin);
	}
	if (currentValue == AlarmOn) {
		//Debug("Alarm!CurrentValue=" + String((int)currentValue));
		actionStatus = ActionStatus::ACTION_ALARM;
	}
	else {
		actionStatus = ActionStatus::ACTION_NORMAL;
	}


	if (oldValue != currentValue) {
		Config.MqttClient()->Publish(this, String(currentValue));
	}

	return true;
}

