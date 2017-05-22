#include "ScriptPump.h"



ScriptPump::ScriptPump(Pump* p, bool enable, String label) : Scenario(enable, label, p)
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
