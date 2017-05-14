#pragma once
#include "DeviceManager.h"
#include "ArduinoServer.h"
#include "ScenarioManager.h"

   
class Configuration
{
public:
	Configuration();
	~Configuration();

	void begin();
	void loop1();
	void loop5();
	void loop10();

	unsigned long counter1 = 0;
	unsigned long counter5 = 0;
	unsigned long counter10 = 0;

	DeviceManager DevMgr = DeviceManager();
	ScenarioManager ScenMgr = ScenarioManager();

private:
	ArduinoServer web = ArduinoServer();
};

