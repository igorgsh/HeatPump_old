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

