#include "ScriptPump.h"



ScriptPump::ScriptPump(Pump* p, bool enable) : Scenario(enable, "", p)
{
	pump = p;
}


ScriptPump::~ScriptPump()
{
}

ScenarioCmd ScriptPump::TriggerredCmd() {
	return ScenarioCmd::SCENARIO_NOCMD;
}

bool ScriptPump::Start() {
	pump->StartPump();
	return true;
}
bool ScriptPump::Stop() {
	pump->StopPump();
	return true;
}
