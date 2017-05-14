#include "Pump.h"



Pump::Pump(String label, int pin, bool on, unsigned long minTimeOn, unsigned long minTimeOff)
{
	r = new Relay(pin, on);
	this->minTimeOn = minTimeOn;
	this->minTimeOff = minTimeOff;
	this->label = label;
}

Pump::~Pump()
{
}

void Pump::StopPump() {
	r->disconnect();
}

void Pump::StartPump() {
	r->connect();
}

void Pump::begin() {
	r->begin();
}

/*
bool Pump::DeviceCommand(ScenarioCommand cmd)
{
	if (cmd == COMMAND_FORCE_STOP) {
		StopPump();
		currentCmd = COMMAND_NO_COMMAND;
		status = STATUS_OFF;
	}
	else if (IsDeviceReady()) {
		if (cmd == COMMAND_STOP) {
			StopPump();
			currentCmd = COMMAND_NO_COMMAND;
			status = STATUS_OFF;
		}
		else if (cmd == COMMAND_START) {
			StartPump();
			currentCmd = COMMAND_NO_COMMAND;
			status = STATUS_ON;
		}
	}
	return true;
}

*/