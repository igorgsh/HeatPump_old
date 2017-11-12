#pragma once
#include "Scenario.h"
#include "Compressor.h"
#include "ScriptPump.h"

#define PUMP_OFF_TIMEOUT 10
#define COMPRESSOR_ON_TIMEOUT 10

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


	bool checkInternalTempConditions();
	bool checkContactors();
	bool IsStartNeeded();
	bool IsForceOff();
};

