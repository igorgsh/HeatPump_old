#include "Relay.h"
#include "Arduino.h"
#include "Definitions.h"

Relay::Relay(int pin, bool on) {
	this->pin = pin;
	this->on = on;

	status = (on==LOW? STATUS_ON : STATUS_OFF);
	//status = STATUS_OFF;
}

Relay::~Relay()
{
}

void Relay::begin() {
	pinMode(pin, OUTPUT);
	disconnect();
}

void Relay::connect() {
	//Debug("Relay connect>>>");
	if (status == STATUS_OFF) {
		//Debug("Done!");
		status = STATUS_ON;
		digitalWrite(pin, on);
	}
}

void Relay::disconnect() {
	//Debug("Relay disconnect<<<");
	if (status == STATUS_ON) {
		//Debug("Done!");
		status = STATUS_OFF;
		digitalWrite(pin, !on);
	}
}
