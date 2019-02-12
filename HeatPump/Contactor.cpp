#include "Contactor.h"
#include "Definitions.h"



Contactor::~Contactor()
{
}


Contactor::Contactor(String label, int pin, bool lhOn, bool alarmOn, int critThreshold)
	: Sensor(label, pin, critThreshold) {
	this->lhOn = lhOn;
	this->AlarmOn = alarmOn;
	init();

}



void Contactor::init() {
	this->type = CONTACT;
	actionPoints[0] = (AlarmOn ? ((float)lhOn) : ((float)!lhOn));
	actionPoints[1] = actionPoints[0];
	actionPoints[2] = (AlarmOn ? ((float)!lhOn) : ((float)lhOn));;
	actionPoints[3] = actionPoints[2];
	digitalWrite(pin, !lhOn);

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
	Debug("AS=" + String(actionStatus));
	//Debug2("CurrentValue=", currentValue);
#else
	currentValue = digitalRead(pin);
#endif // _SIMULATOR_
	return true;
}
