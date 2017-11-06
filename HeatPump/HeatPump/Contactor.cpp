#include "Contactor.h"
#include "Definitions.h"



Contactor::~Contactor()
{
}


Contactor::Contactor(String label, int pin, bool lhOn, float actionPoints[], int critThreshold)
	: Sensor(label, pin, actionPoints, critThreshold) {
	this->lhOn = lhOn;
	init();

}



void Contactor::init() {
	this->type = CONTACT;
/*
	if (Alarm) {
		actionPoints[0] = -1.0;
		actionPoints[1] = -1.0;
		actionPoints[2] = 0.5;
		actionPoints[3] = 0.5;
	}
	else {
		actionPoints[0] = 0.5;
		actionPoints[1] = 0.5;
		actionPoints[2] = 1.5;
		actionPoints[3] = 1.5;
	}
	*/
	digitalWrite(pin, !lhOn);

}

void Contactor::begin() {
}

bool Contactor::loop(unsigned long counter) {
	bool res;

	res = getData();
	return res;
}

bool Contactor::checkDataReady() {
	//Debug("Point5");
#ifdef _SIMULATOR_
	//Debug("Point4");
	currentValue = sim->GetRealResult(pin);
	//Debug2("CurrentValue=", currentValue);
#else
	currentValue = digitalRead(pin);
#endif // _SIMULATOR_
	return true;
}
