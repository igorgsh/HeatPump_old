#pragma once
//#include "EEPROM.h"
#include "DeviceManager.h"
#include "ArduinoServer.h"
#include "ScriptManager.h"
#include "MqttCredentials.h"

//#define WEB_ENABLED
//#undef WEB_ENABLED

#define EEPROM_ID	0x00  //1 bytes
#define EEPROM_DESIRED_TEMP	0x01  //1 bytes
#define EEPROM_MQTT			0x14	//Initial addr is 0x14=20

#define TEMPERATURE_DELTA	5

class Configuration
{
public:
	Configuration();
	~Configuration();

	void begin();
	void loop();

	unsigned long Counter100 = 0;
	unsigned long Counter500 = 0;
	unsigned long Counter1s = 0;

	DeviceManager DevMgr = DeviceManager();
	ScriptManager ScriptMgr = ScriptManager();

	float GetDesiredTemp() /*{ return desiredTemp; }*/;
	void SetDesiredTemp(float value);
	float GetCurrentTemp() { return DevMgr.currentTemp->getValue(); }
	float OutTemp();
	float ControlTemperature() { return DevMgr.currentTemp->getValue(); };
	byte GetBoardId() { return boardId; };
	MqttCredentials GetMqttCreds() { return MqttCreds; }
	String PrintIP(IPAddress addr);

	// Check methods
	bool IsReady() { return isReady; }

private:
	// Flag variables
	bool isReady = false;
	

	ArduinoServer web = ArduinoServer();
	float desiredTemp;
	void EepromWrite(unsigned int addr, byte value);
	void EepromWrite(unsigned int addr, unsigned int value);
	byte EepromRead(unsigned int addr);
	unsigned int EepromRead2(unsigned int addr);

	void ReadEepromInfo();
	void ReadMqttCredentials();
	void WriteMqttCredentials();

	byte boardId;
	MqttCredentials MqttCreds;
	//Modules configuraton

	const bool availMqttClient = true;

};

