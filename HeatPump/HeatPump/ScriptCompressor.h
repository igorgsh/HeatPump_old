#pragma once
#include "Scenario.h"
#include "Compressor.h"
#include "ScriptPump.h"

class ScriptCompressor :
	public Scenario
{
public:
	ScriptCompressor(Compressor* compressor, bool enable, String label, Scenario* pGeo, Scenario* p1, Scenario* p2 );
	~ScriptCompressor();
	virtual ScenarioCmd TriggerredCmd();
	bool Start();
	bool Stop();

	bool begin() {return true;};


private:
	Compressor *comp;

	ScriptPump* pumpGeo;
	ScriptPump* pumpContour1;
	ScriptPump* pumpContour2;

	//bool StartCompressor();
	//bool StopCompressor();
	
};

