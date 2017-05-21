#include "Relay.h"
#include "Arduino.h"

Relay::Relay(int pin, bool on) {
	this->pin = pin;
	this->on = on;

	status = (on==LOW? STATUS_ON : STATUS_OFF);
}

Relay::~Relay()
{
}

void Relay::begin() {
	pinMode(pin, OUTPUT);
	disconnect();
	digitalWrite(pin, status);
}

void Relay::connect() {
	if (status == STATUS_OFF) {
		status = STATUS_ON;
		digitalWrite(pin, on);
	}
}

void Relay::disconnect() {
	if (status == STATUS_ON) {
		status = STATUS_OFF;
		digitalWrite(pin, !on);
	}
}
