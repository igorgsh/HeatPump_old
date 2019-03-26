#pragma once
//#include "EEPROM.h"
#include "Mqtt.h"
#include "DeviceManager.h"
#include "ArduinoServer.h"
#include "ScriptManager.h"
#include "MqttCredentials.h"

//EEPROM STRUCTURE
#define EEPROM_ID	0x00  //1 bytes
#define EEPROM_DESIRED_TEMP	0x01  //1 bytes
#define EEPROM_T_ADDRESS 0x14 // 8* bytes
#define EEPROM_MQTT			100	//Initial addr is 100
/*
addr   |len|value
--------------
 0     |1  |BoardId
 1     |1  |Desired Temp
 2-19  |18 |Reserved
20-27  |8  |T1 Address
28-35  |8  |T2 Address
36-43  |8  |T3 Address
44-51  |8  |T4 Address
52-59  |8  |T5 Address
60-67  |8  |T6 Address
68-75  |8  |T7 Address
76-99  |24 |Reserved
100-103|4  |MQTT IP
104-105|2  |MQTT Port
106-106|1  |Length of login (LL)
107-...|LL |Login
...-...|1  |Length of Password (LP)
...-...|LP |Password


*/

#define TEMPERATURE_DELTA	5

class Configuration
{
public:
	Configuration();
	//~Configuration();

	void setup();
	void loop();

	unsigned long Counter100 = 0;
	unsigned long Counter500 = 0;
	unsigned long Counter1s = 0;

	DeviceManager DevMgr = DeviceManager();
	ScriptManager ScriptMgr = ScriptManager();


	float GetDesiredTemp() /*{ return desiredTemp; }*/;
	void SetDesiredTemp(float value);
	float GetCurrentTemp() { return DevMgr.currentTemp->GetValue(); }
	float OutTemp();
	float GetControlTemperature() { return DevMgr.currentTemp->GetValue(); };
	//void SetControlTemperature(float value;) { }
	byte GetBoardId() { return boardId; };
	String BoardName() { return "HeatPump_" + String(boardId); }
	MqttCredentials GetMqttCreds() { return mqttCreds; }
	String PrintIP(IPAddress addr);
	Mqtt* GetMqttClient() { return mqttClient; }
	// Check methods
	bool IsReady() { return isReady; }

	//Modules configuration Checks 
	bool IsAutoTesting() { return availSimulator & availAutoTesting; };
	bool IsEthernet() { return isEthernetReady;}
	bool IsSimulator() { return availSimulator; }
	bool IsWeb() { return isWebReady; /* availEthernet & availWebServer;*/ }
	bool IsMqtt() { return isMqttReady;/*availEthernet & availMqttClient;*/ }
	String /*static*/ PrintMac(byte* devAddr, int format=1);

private:
	// Flag variables
	bool isReady = false;
	bool isEthernetReady = false;
	bool isMqttReady = false;
	bool isWebReady = false;

	byte mac[6] = { 0x02, 0xAA, 0x22, 0x07, 0x69, 0x07 };

	void ethernetSetup();
	ArduinoServer* web;
	float desiredTemp;
	void eepromWrite(unsigned int addr, byte value[8]);
	void eepromWrite(unsigned int addr, byte value);
	void eepromWrite(unsigned int addr, unsigned int value);
	byte eepromRead(unsigned int addr);
	unsigned int eepromRead2(unsigned int addr);

	void readEepromInfo();
	void readMqttCredentials();
	void writeMqttCredentials();

	byte boardId;
	MqttCredentials mqttCreds;
	Mqtt* mqttClient;

	//Modules configuraton

	const bool availMqttClient = true;
	const bool availAutoTesting = true;
	const bool availEthernet = true;
	const bool availSimulator = true;
	const bool availWebServer = true;

};

