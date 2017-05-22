#include "Scenario.h"
#include "ScenarioManager.h"


void Scenario::generateId() {
	id = micros() % 0xFFFF;
}

void Scenario::generateLabel() {
	label = "S" + id;
}

Scenario::Scenario(bool enable, String label, OutputDevice* dev )
{
	device = dev;
	Enabled = enable;
	generateId();
	if (label.length() != 0) {
		this->label = label;
	}
	else {
		generateLabel();
	}

	//this->scenMgr = (ScenarioManager*)ScenMgr;
}


Scenario::~Scenario()
{
}


bool Scenario::Run(ScenarioCmd cmd, unsigned long counter) {

	Debug("ScenarioRun");
	bool res = false;
	if (cmd == ScenarioCmd::SCENARIO_START) {
		Debug("CmdStart");
		if (device->status == STATUS_ON) { // Nothing to do. Device is running
			Debug("Status_on");
			res = true;
		}
		else { //Device is off of unknown
			Debug("Status_off");

			if (device->lastStatusTimestamp + device->minTimeOff <= counter) { //Device is off a long time. Start it
				Debug("Get started");
				res = Start();
			}
			else { //Device is Off but timeout is still not over
				Debug("Nothing");
				res = false;
			}
		}
	}
	else if (cmd == ScenarioCmd::SCENARIO_STOP) {
		if (device->status == STATUS_OFF) { // Nothing to do. Device is stoped 
			res = true;
		}
		else { //Device is on of unknown
			if (device->lastStatusTimestamp + device->minTimeOn <= counter) { //Device is on a long time. Stop it
				res=Stop();
			}
			else { //Device is On but timeout is still not over
				res = false;
			}
		}
	}
	else {//unknown command
		res = true;
	}
	Debug("ScenarioStart_End")
	return res;
}
