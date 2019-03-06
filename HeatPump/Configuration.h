#pragma once

//#define ETHERNET_ENABLED
//#define MQTT_ENABLED
//#define WEB_ENABLED
//#define SDCARD_ENABLED

#define SDCARD_SS	4
#define DHCP_TIMEOUT 10000UL

//#include "EEPROM.h"
#include "Loger.h" 
#include "DeviceManager.h"
#include "ArduinoServer.h"
#include "ScriptManager.h"
#include "MqttCredentials.h"
#include "Mqtt.h"
	/*
	EEPROM:
	Address	Byte	Comment
	0		1		ID board
	1		1		Desired temp: T*2
	Mqtt Block
	20		2			Port of MQTT server
	22		1		n1	Length of Server URL (up to 50 chars)
	23		n1			Server URL
			1		n4	Length of Root
			n4			Root
			1		n2	Length of login
			n2			Login
			1		n3	Length of password
			n3			Password
	*/

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

	unsigned long counter100 = 0;
	unsigned long counter500 = 0;
	unsigned long counter1s = 0;
	bool isHardwareReady = false;

	DeviceManager DevMgr = DeviceManager();
	ScriptManager ScriptMgr = ScriptManager();
	float GetDesiredTemp();
	void SetDesiredTemp(float value);
	float getTemp() { return DevMgr.currentTemp->getValue(); }
	float OutTemperature();
	float ControlTemperature() { return DevMgr.currentTemp->getValue(); };
	int GetId() {return id;	};
	MqttCredentials GetMqttCredentials() { return MqttCreds; }
	Mqtt* MqttClient() { return mqttClient; }
	bool IsAutoTesting() { return autoTesting; }
	bool IsSimulator() { return simulator;  }
	bool IsEthernetConnection() { return isEthernetConnection; }
	bool IsWebServerAvailable() { return webServer; }
	bool IsMqttAvailable() { return mqttClientAvail; }
	String BoardName() { return boardName; }
	String PrintIP(IPAddress addr);

private:
	byte mac[6] = { 0x00, 0xAA, 0x22, 0x07, 0x69, 0x07 };
	//byte mac[6] = { 0xAB, 0xBC, 0x18, 0xF5, 0xD4, 0xA6 };
	IPAddress staticIP = IPAddress(192, 168, 0, 101);
	String boardName = "HeatPump_";
	int id = 0;
	ArduinoServer web = ArduinoServer();
	Mqtt *mqttClient;
	float desiredTemp = 0;
	//byte getDesiredTemp();
	void setDesiredTemp(byte value);
	void EepromWrite(unsigned int addr, byte value);
	void EepromWrite(unsigned int addr, unsigned int value);
	byte EepromRead(unsigned int addr);
	unsigned int EepromRead2(unsigned int addr);
	bool isEthernetConnection = false;

	MqttCredentials MqttCreds;
	
	void ReadEepromInfo();
	void ReadMqttCredentials();
	void WriteMqttCredentials();

	void EthernetSetup();

// Configuration Definition
	const bool autoTesting = true;
	const bool ethernet = true;
	const bool simulator = true;
	const bool webServer = false;
	const bool mqttClientAvail = false;

};

//extern Simulator* sim;
//extern AutoTests* test;
//extern Configuration Config;
