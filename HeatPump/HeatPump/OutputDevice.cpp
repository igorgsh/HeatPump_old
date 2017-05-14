#include "OutputDevice.h"



OutputDevice::OutputDevice()
{
}


OutputDevice::~OutputDevice()
{
}
/*
void OutputDevice::Command(ScenarioCommand cmd)
{
	if (currentCmd != COMMAND_NO_COMMAND) { // last command doesn't completed
		if (currentCmd != cmd) { //and previous command is differ else nothing to do
			if ((cmd == COMMAND_FORCE_STOP || cmd == COMMAND_STOP) && status!=STATUS_OFF) { // run STOP when Device is not OFF else command is ignored 
				if (cmd == COMMAND_STOP && status == STATUS_ON && lastStatusTimestamp+minTimeOn>Config.counter1) { //wait until minTimeOn
					WaitTime(lastStatusTimestamp + minTimeOn - Config.counter1);
				}
				else {
					DeviceCommand(cmd);
				}
			}
		}
	} else { //device is ready to accept command START
		if (cmd == COMMAND_START && status == STATUS_OFF && lastStatusTimestamp + minTimeOff > Config.counter1) { //wait until minTimeOff
			WaitTime(lastStatusTimestamp + minTimeOff - Config.counter1);
		}
		else {
			DeviceCommand(cmd);
		}
	}
}
*/
/*
void OutputDevice::WaitTime(unsigned long delay, void(*callBack)())
{
	this->callBack = callBack;
}


void OutputDevice::WaitTime(unsigned long delay)
{
	this->callBack = NULL;
	if (delay > 0) {
		nextDeviceReady = Config.counter1 + delay;
	}
}
*/