#pragma once
#include "Script.h"
#include "Compressor.h"
#include "ScriptPump.h"

#define PUMP_OFF_TIMEOUT 5 //3*60
#define COMPRESSOR_ON_TIMEOUT 5 //1*60

typedef enum {
	COMPRESSOR_IDLE = 0,
	COMPRESSOR_IS_START_NEEDED = 1,
	COMPRESSOR_START_PUMP_GEO = 2,
	COMPRESSOR_WAITING_PUMP_GEO_START = 3,
	COMPRESSOR_START = 4,
	COMPRESSOR_IS_STOP_NEEDED = 5,
	COMPRESSOR_STOP = 6,
	COMPRESSOR_WAITING_PUMP_GEO_STOP = 7,
	COMPRESSOR_STOP_PUMP_GEO = 8,
	COMPRESSOR_ALARM_DELAY = 99
} COMPRESSOR_STEPS;

class ScriptHeatPump :
	public Script
{
public:
	ScriptHeatPump(bool enable, String label, unsigned int alarmDelay);
	~ScriptHeatPump();
	bool Start(bool isSync) { return MainLoop(isSync); }
	bool Stop(bool isSync);
	//bool IsStartAllowed(bool isSync);
	//bool IsStopAllowed(bool isSync);
	bool ForceStop();
	//bool ForceStart();

	bool MainLoop(bool isSync);
	bool IsAlarm();

private:


	bool checkTempConditions();
	bool checkContactors();
	bool IsStartNeeded(bool isSync);
	bool IsStopNeeded(bool isSync);
	bool checkAllConditions();
	bool StopCompressor(bool isSync);
	bool StartCompressor(bool isSync);
};

