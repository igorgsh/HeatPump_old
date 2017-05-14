#include "ScriptPump.h"



ScriptPump::ScriptPump(Pump* p)
{
	pump = p;
}


ScriptPump::~ScriptPump()
{
}

ScenarioCmd ScriptPump::TriggerredCmd() {
	return ScenarioCmd::SCENARIO_NOCMD;
}

