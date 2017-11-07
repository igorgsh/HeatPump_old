#include "Scenario.h"
#include "ScenarioManager.h"
#include "Configuration.h"

extern Configuration Config;


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
}


Scenario::~Scenario()
{
}


bool Scenario::Run(ScenarioCmd cmd) {

	//Debug("ScenarioRun");
	bool res = false;
	if (cmd == ScenarioCmd::SCENARIO_START) {
		Debug("CmdStart");
		if (device->status == STATUS_ON) { // Nothing to do. Device is running
			Debug("Status_on");
			res = true;
		}
		else { //Device is off or unknown
			Debug("Status_off");

			if (device->lastStatusTimestamp + device->minTimeOff <= Config.counter1s) { //Device is off a long time. Start it
				Debug("Get started");
				res = Start();
			}
			else { //Device is Off but timeout is still not over
				Debug("Start timeout! Counter=" + String() + ";LastTs=" + String(device->lastStatusTimestamp) + ";mintime=" + String(device->minTimeOff));
				//Debug("Nothing");
				res = false;
			}
		}
	}
	else if (cmd == ScenarioCmd::SCENARIO_STOP) {
		Debug("CmdStop");
		if (device->status == STATUS_OFF) { // Nothing to do. Device is stoped 
			Debug("Already stoped!");
			res = true;
		}
		else { //Device is on of unknown
			if (device->lastStatusTimestamp + device->minTimeOn <= Config.counter1s) { //Device is on a long time. Stop it
				Debug("Run Stop");
				res=Stop();
			}
			else { //Device is On but timeout is still not over
				Debug("Stop timeout! Counter=" + String(Config.counter1s) + ";LastTs=" + String(device->lastStatusTimestamp) + ";mintime=" + String(device->minTimeOn));
				res = false;
			}
		}
	}
	else {//unknown command
		res = true;
	}
	//Debug("ScenarioStart_End")
	return res;
}
