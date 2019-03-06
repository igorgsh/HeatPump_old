#include "Configuration.h"
#include "Definitions.h"
#include "EEPROM.h"
#include <SD.h>

extern Simulator* sim;

Configuration::Configuration()
{
}


Configuration::~Configuration()
{
}

void Configuration::loop() {
	//Debug("Loop:" + String(counter1s));
	DevMgr.loop();
	if (isHardwareReady) {
		ScriptMgr.loop();
	}
	if (IsMqttAvailable()) {
		mqttClient->loop();
	}
	if (IsWebServerAvailable()) {
		web.loop();
	}
}

void Configuration::EthernetSetup() {

	mac[5] = id;
	boardName += id;
	IPAddress ip(192, 168, 0, 101);

	Loger::Debug("Boardname = " + boardName);

	if (Ethernet.begin(mac, DHCP_TIMEOUT) == 0) {
		Loger::Debug("Failed to configure Ethernet using DHCP");
		// no point in carrying on, so do nothing forevermore:
		// try to congifure using IP address instead of DHCP:
		Ethernet.begin(mac, ip);
	}
	
	Loger::Debug("Server is at: " + PrintIP(Ethernet.localIP()));
	isEthernetConnection = true;
	//isEthernetConnection = Ethernet.linkStatus() == EthernetLinkStatus::LinkON;
	//Loger::Debug("IsEth=" + String(Ethernet.linkStatus()));
}


void Configuration::begin() {
	ReadEepromInfo();

	Loger::Debug("Point 2");

	if (ethernet) {
		EthernetSetup();
	}
	Loger::Debug("Point 3");

	DevMgr.begin();

	if (webServer) {
		Loger::Info("Web server is starting...");
		web.begin();
	}
	if (mqttClientAvail) {
		Loger::Info("MQTT is starting...");
		mqttClient = new Mqtt();
		mqttClient->begin();
	}
}

void Configuration::ReadEepromInfo() {
	byte b;

	// Read Id
	id = EepromRead(EEPROM_ID);
	// Read desired temp
	b = EepromRead(EEPROM_DESIRED_TEMP);
	setDesiredTemp(b);

	//Read MQTT Credentials
	if (mqttClientAvail) {
		ReadMqttCredentials();
	}
}

void Configuration::ReadMqttCredentials() {
	byte len;
	unsigned addr = EEPROM_MQTT;
	char* buf;

	for (int i = 0; i < 4; i++) {
		MqttCreds.ServerIP[i] = EepromRead(addr);
		addr++;
	}

	MqttCreds.Port = EepromRead2(addr);
	addr += 2;

	len = EepromRead(addr);
	addr += 1;
	buf = (char*)malloc(len + 1);

	for (int i = 0; i < len; i++) {
		buf[i] = EepromRead(addr);
		addr++;
	}
	buf[len] = 0;
	MqttCreds.Root = String(buf);
	free(buf);


	len = EepromRead(addr);
	addr += 1;
	if (len != 0) {
		buf = (char*)malloc(len + 1);

		for (int i = 0; i < len; i++) {
			buf[i] = EepromRead(addr);
			addr++;
		}
		buf[len] = 0;
		MqttCreds.Login = String(buf);
		free(buf);
	}
	else {
		MqttCreds.Login = "";
	}
	len = EepromRead(addr);
	addr += 1;
	if (len != 0) {
		buf = (char*)malloc(len + 1);

		for (int i = 0; i < len; i++) {
			buf[i] = EepromRead(addr);
			addr++;
		}
		buf[len] = 0;
		MqttCreds.Password = String(buf);
		free(buf);

		MqttCreds.Password[len] = 0;
	}
	else {
		MqttCreds.Password = "";
	}
/*
	Debug("EEPROM:MQTT:URL:" + MqttCreds.ServerURL);
	Debug("EEPROM:MQTT:Port:" + String(MqttCreds.Port));
	Debug("EEPROM:MQTT:Root:" + String(MqttCreds.Root));
	Debug("EEPROM:MQTT:Login:" + String(MqttCreds.Login));
	Debug("EEPROM:MQTT:Password:" + String(MqttCreds.Password));
	*/
}


void Configuration::WriteMqttCredentials() {
	byte len;
	unsigned addr = EEPROM_MQTT;

	for (int i = 0; i < 4; i++) {
		EepromWrite(addr, MqttCreds.ServerIP[i]);
		addr++;
	}

	EepromWrite(addr, (unsigned)MqttCreds.Port);
	addr += 2;

	len = MqttCreds.Root.length();
	EepromWrite(addr, len);
	addr += 1;
	for (int i = 0; i < len; i++) {
		EepromWrite(addr, (byte)MqttCreds.Root[i]);
		addr++;
	}

	len = MqttCreds.Login.length();
	EepromWrite(addr, len);
	addr += 1;
	if (len != 0) {
		for (int i = 0; i < len; i++) {
			EepromWrite(addr, (byte)MqttCreds.Login[i]);
			addr++;
		}
	}

	len = MqttCreds.Password.length();
	EepromWrite(addr, len);
	addr += 1;
	if (len != 0) {
		for (int i = 0; i < len; i++) {
			EepromWrite(addr, (byte)MqttCreds.Password[i]);
			addr++;
		}
	}
}



float Configuration::OutTemperature() {
	float outTemp = 0;
	if (desiredTemp <= 20)
		outTemp = 25;
	else if (desiredTemp >= 45)
		outTemp = 45;
	else
		outTemp = desiredTemp + TEMPERATURE_DELTA;
	return outTemp;
}

void Configuration::EepromWrite(unsigned int addr, byte value) {

	EEPROM.write(addr, value);
}

void Configuration::EepromWrite(unsigned int addr, unsigned int value) {

	EEPROM.write(addr, value & 0xFF);
	EEPROM.write(addr + 1, (value >> 8) & 0xFF);
}

byte Configuration::EepromRead(unsigned int addr) {
	return EEPROM.read(addr);
}

unsigned int Configuration::EepromRead2(unsigned int addr) {
	byte x;
	byte b;

	x = EEPROM.read(addr);
	b = EEPROM.read(addr + 1);
	return (unsigned)b << 8 | x;
}


void Configuration::setDesiredTemp(byte value) {

	float fv = value / 2.0;

	if (desiredTemp != fv) { //optimization: reduce number of write to EEPROM

		desiredTemp = fv;
		EepromWrite(EEPROM_DESIRED_TEMP, value);
	}
}

void Configuration::SetDesiredTemp(float value) {

	byte bv = (byte)(value * 2);

	if (desiredTemp != value) { //optimization: reduce number of write to EEPROM

		desiredTemp = value;
		EepromWrite(EEPROM_DESIRED_TEMP, bv);
	}
}


float Configuration::GetDesiredTemp() {
	if (IsSimulator()) {
		return (byte)(sim->GetIntResult(99));
	}
	else {
		return desiredTemp;
	}
}

String Configuration::PrintIP(IPAddress addr) {
	return String(addr[0]) + "." + String(addr[1]) + "." + String(addr[2]) + "." + String(addr[3]);
}

