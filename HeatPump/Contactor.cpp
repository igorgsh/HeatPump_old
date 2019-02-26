#include "Contactor.h"
#include "Definitions.h"



Contactor::~Contactor()
{
}


Contactor::Contactor(String label, int pin, bool lhOn, bool alarmOn)
	: Sensor(label, pin) {
	this->lhOn = lhOn;
	this->AlarmOn = alarmOn;
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
	//Debug("Point5");
#ifdef _SIMULATOR_
	//Debug("Point4");
	currentValue = sim->GetRealResult(pin);
	//Debug("AS=" + String(actionStatus));
	//Debug2("CurrentValue=", currentValue);
#else
	currentValue = digitalRead(pin);
#endif // _SIMULATOR_
	return true;
}

