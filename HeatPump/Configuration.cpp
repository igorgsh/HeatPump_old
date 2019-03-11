#include "Configuration.h"
#include "Definitions.h"
#include "EEPROM.h"

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
	if (Ethernet.begin(mac) == 0) {
		Loger::Debug("Failed to configure Ethernet using DHCP");
		// no point in carrying on, so do nothing forevermore:
		// try to congifure using IP address instead of DHCP:
		IPAddress ip(192, 168, 0, 100+id);
		Ethernet.begin(mac, ip);
	}
	Loger::Debug("Server is at: " + String(Ethernet.localIP()));
	isEthernetConnection = Ethernet.linkStatus() == EthernetLinkStatus::LinkON;

}


void Configuration::begin() {
	ReadEepromInfo();

	if (ethernet) {
		EthernetSetup();
	}
	DevMgr.begin();

	if (webServer) {
		Loger::Info("Server Is Starting...");
		web.begin();
	}
	if (mqttClientAvail) {

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
	MqttCreds.ServerURL = String(buf);
	free(buf);

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

	EepromWrite(addr, (unsigned)MqttCreds.Port);
	addr += 2;
	len = MqttCreds.ServerURL.length();
	EepromWrite(addr, len);
	addr += 1;
	for (int i = 0; i < len; i++) {
		EepromWrite(addr, (byte)MqttCreds.ServerURL[i]);
		addr++;
	}

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
