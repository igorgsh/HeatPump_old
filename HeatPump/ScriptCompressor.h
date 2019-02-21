#pragma once
#include "Script.h"
#include "Compressor.h"
#include "ScriptPump.h"

#define PUMP_OFF_TIMEOUT 5 //3*60
#define COMPRESSOR_ON_TIMEOUT 5 //1*60

typedef enum {
	COMPRESSOR_IDLE = 0,
	COMPRESSOR_IS_START_NEEDED =1,
	COMPRESSOR_START_PUMP_GEO = 2,
	COMPRESSOR_WAITING_PUMP_GEO_START = 3,
	COMPRESSOR_START = 4,
	COMPRESSOR_IS_STOP_NEEDED =5,
	COMPRESSOR_STOP=6,
	COMPRESSOR_WAITING_PUMP_GEO_STOP=7,
	COMPRESSOR_STOP_PUMP_GEO=8
} COMPRESSOR_STEPS;

class ScriptCompressor :
	public Script
{
public:
	ScriptCompressor(Compressor* compressor, bool enable, String label, Script* pGeo, Script* p1, Script* p2 );
	~ScriptCompressor();
	//virtual ScenarioCmd TriggerredCmd();
	bool Start(bool isSync);
	bool Stop(bool isSync);

	bool begin() {return true;};


private:
	//Compressor *comp;

	//ScriptPump* pumpGeo;
	//ScriptPump* pumpContour1;
	//ScriptPump* pumpContour2;


	bool checkInternalTempConditions();
	bool checkContactors();
	bool IsStartNeeded();
	bool IsStopNeeded();
	//bool IsForceOff();
	bool checkAllConditions();
	bool StopCompressor(bool isSync);
	bool StartCompressor(bool isSync);
};

