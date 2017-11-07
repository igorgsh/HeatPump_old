#pragma once
//#include "EEPROM.h"
#include "DeviceManager.h"
#include "ArduinoServer.h"
#include "ScenarioManager.h"

//#define WEB_ENABLED
//#undef WEB_ENABLED

#define EEPROM_Desired_Temp	0x00  //1 bytes


class Configuration
{
public:
	Configuration();
	~Configuration();

	void begin();
	void loop();

	unsigned long counter100 = 0;
	unsigned long counter500 = 0;
	unsigned long counter1s = 0;

	DeviceManager DevMgr = DeviceManager();
	ScenarioManager ScenMgr = ScenarioManager(&DevMgr);
	byte getDesiredTemp() { return desiredTemp; }
	void setDesiredTemp(byte value);
	float getTemp() { return DevMgr.currentTemp->getValue(); }
	float OutTemperature();
private:
	ArduinoServer web = ArduinoServer();
	byte desiredTemp=EepromRead(EEPROM_Desired_Temp);
	void EepromWrite(unsigned int addr, byte value);
	void EepromWrite(unsigned int addr, unsigned int value);
	byte EepromRead(unsigned int addr);
	unsigned int EepromRead2(unsigned int addr);
};

