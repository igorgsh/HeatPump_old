#include "Configuration.h"
#include "Definitions.h"
#include "EEPROM.h"

#define WEB_ENABLED


Configuration::Configuration()
{
}


Configuration::~Configuration()
{
}

void Configuration::loop() {
	DevMgr.loop();
	if (isHardwareReady) {
		ScriptMgr.loop();
	}
#ifdef WEB_ENABLED
	web.loop();
#endif // WEB_ENABLED
}

void Configuration::begin() {

	ReadEepromInfo();

	DevMgr.begin();

#ifdef WEB_ENABLED
	Debug("Server Is Starting...");
	web.begin();
#endif
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

	EEPROM.write(addr, value % 0xFF);
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

	if (desiredTemp != value) { //optimization: reduce number of write to EEPROM
		desiredTemp = value;
		EepromWrite(EEPROM_DESIRED_TEMP, desiredTemp);
	}
}

byte Configuration::getDesiredTemp() {
#ifdef _SIMULATOR_
	return (byte)(sim->GetIntResult(99));
#else
	return desiredTemp;
#endif // _SIMULATOR_

}

void Configuration::ReadEepromInfo() {
	byte b;

	// Read Id
	boardId = EepromRead(EEPROM_ID);
	// Read desired temp
	b = EepromRead(EEPROM_DESIRED_TEMP);
	setDesiredTemp(b);

	Debug("BoardId=" + String(boardId));

	//Read MQTT Credentials
	if (availMqttClient) {
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
	
		Debug("EEPROM:MQTT:URL:" +  PrintIP(MqttCreds.ServerIP));
		Debug("EEPROM:MQTT:Port:" + String(MqttCreds.Port));
		Debug("EEPROM:MQTT:Root:" + String(MqttCreds.Root));
		Debug("EEPROM:MQTT:Login:" + String(MqttCreds.Login));
		Debug("EEPROM:MQTT:Password:" + String(MqttCreds.Password));
		
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


String Configuration::PrintIP(IPAddress addr) {
	return String(addr[0]) + "." + String(addr[1]) + "." + String(addr[2]) + "." + String(addr[3]);
}
