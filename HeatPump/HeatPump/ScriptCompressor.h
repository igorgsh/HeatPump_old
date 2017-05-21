#pragma once
#include "Scenario.h"
#include "Compressor.h"
#include "ScriptPump.h"

class ScriptCompressor :
	public Scenario
{
public:
	ScriptCompressor(Compressor* compressor, bool enable, Scenario* pGeo, Scenario* p1, Scenario* p2 );
	~ScriptCompressor();
	ScenarioCmd TriggerredCmd();
	bool Start();
	bool Stop();
private:
	Compressor *comp;

	ScriptPump* pumpGeo;
	ScriptPump* pumpContour1;
	ScriptPump* pumpContour2;

	bool StartCompressor();
	bool StopCompressor();
	
};

