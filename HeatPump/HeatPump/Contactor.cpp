#include "Contactor.h"




Contactor::~Contactor()
{
}


Contactor::Contactor(String label, int pin, bool alarm, int critThreshold)
	: Sensor(label, pin, new float[NUMBER_OF_ACTIONPOINTS] {-1.0, -1.0, 2.0, 2.0}, critThreshold) {
	Alarm = alarm;
	init();

}



void Contactor::init() {
	this->type = CONTACT;
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
	digitalWrite(pin, !Alarm);

}

void Contactor::begin() {
}

bool Contactor::loop(unsigned long counter) {
	return true;
}

bool Contactor::checkDataReady() {
	currentValue = digitalRead(pin);
	return true;
}


/*
void TempSensor::requestTemperatures() {
	dt->requestTemperatures();
}


*/