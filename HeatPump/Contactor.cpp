#include "Contactor.h"
#include "Definitions.h"



Contactor::~Contactor()
{
}


Contactor::Contactor(String label, int pin, bool lhOn, bool alarmOn)
	: Sensor(label, pin) {
	this->lhOn = lhOn;
	this->AlarmOn = alarmOn;
	this->lowerRange = lhOn;
	this->upperRange = lhOn;
	init();

}



void Contactor::init() {
	this->type = CONTACT;
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
#ifdef _SIMULATOR_
	currentValue = sim->GetRealResult(pin);
#else
	currentValue = digitalRead(pin);
#endif // _SIMULATOR_
	if (currentValue == AlarmOn) {
		//Debug("Alarm!CurrentValue=" + String((int)currentValue));
		actionStatus = ActionStatus::ACTION_ALARM;
	}
	else {
		actionStatus = ActionStatus::ACTION_NORMAL;
	}

	return true;
}

