#include "Scenario.h"
#include "ScenarioManager.h"


void Scenario::generateId() {
	id = micros() % 0xFFFF;
}

void Scenario::generateLabel() {
	label = "S" + id;
}

Scenario::Scenario(bool enable, String label )
{
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
