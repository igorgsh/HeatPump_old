#include "Script.h"
#include "ScriptManager.h"
#include "Configuration.h"

extern Configuration Config;


void Script::generateId() {
	id = micros() % 0xFFFF;
}

void Script::generateLabel() {
	label = "S" + id;
}

Script::Script(bool enable, String label, unsigned int alarmDelay/*, OutputDevice* dev*/)
{
	//device = dev;
	this->alarmDelay = alarmDelay;
	Enabled = enable;
	generateId();
	
	if (label.length() != 0) {
		this->label = label;
	}
	else {
		generateLabel();
	}
}


Script::~Script()
{
}

bool Script::Run(ScenarioCmd cmd) {
	bool res = false;
	switch (cmd) {
	case ScenarioCmd::SCENARIO_NOCMD: {
		res = true;
		break;
	}
	case ScenarioCmd::SCENARIO_START: {
		res = Start(false);
		break;
	}
	case ScenarioCmd::SCENARIO_STOP: {
		res = Stop(false);
		break;
	}
	case ScenarioCmd::SCENARIO_FORCE_STOP: {
		res = ForceStop();
		break;
	}
	case ScenarioCmd::SCENARIO_FORCE_START: {
		res = ForceStart();
		break;
	}
	}
	if (res) { // The command is completed
		step = 0;
		lastCmd = ScenarioCmd::SCENARIO_NOCMD;
	}

	return res;
}
