#include "Configuration.h"
#include "Definitions.h"
#include "EEPROM.h"



Configuration::Configuration()
{
}



void Configuration::loop() {
	DevMgr.loop();
	ScriptMgr.loop();
	if (IsWeb()) {
		web.loop();
	}
}

void Configuration::setup() {

	readEepromInfo();

	if (IsEthernet()) {
		ethernetSetup();
	}
	DevMgr.begin();
	isReady = ScriptMgr.setup();

	if (IsWeb()) {
		Debug("Web Server Is Starting...");
		web.begin();
	}
}

float Configuration::OutTemp() {
	float outTemp = 0;
	if (desiredTemp <= 20)
		outTemp = 25;
	else if (desiredTemp >= 45)
		outTemp = 45;
	else
		outTemp = desiredTemp + TEMPERATURE_DELTA;
	return outTemp;
}

void Configuration::eepromWrite(unsigned int addr, byte value) {

	EEPROM.write(addr, value);
}

void Configuration::eepromWrite(unsigned int addr, unsigned int value) {

	EEPROM.write(addr, value % 0xFF);
	EEPROM.write(addr + 1, (value >> 8) & 0xFF);
}

byte Configuration::eepromRead(unsigned int addr) {
	return EEPROM.read(addr);
}

unsigned int Configuration::eepromRead2(unsigned int addr) {
	byte x;
	byte b;

	x = EEPROM.read(addr);
	b = EEPROM.read(addr + 1);
	return (unsigned)b << 8 | x;
}


void Configuration::SetDesiredTemp(float value) {

	if (desiredTemp != value) { //optimization: reduce number of write to EEPROM
		desiredTemp = value;
		eepromWrite(EEPROM_DESIRED_TEMP, (byte)(desiredTemp*2));
	}
}

float Configuration::GetDesiredTemp() {
	if (IsSimulator()) {
		return sim->GetRealResult(99);
	}
	else {
		return desiredTemp;
	}
}

void Configuration::readEepromInfo() {
	byte b;

	// Read Id
	boardId = eepromRead(EEPROM_ID);
	// Read desired temp
	b = eepromRead(EEPROM_DESIRED_TEMP);
	desiredTemp = ((float)b)/2.0;

	Debug("BoardId=" + String(boardId));

	//Read MQTT Credentials
	if (availMqttClient) {
		readMqttCredentials();
	}
}

void Configuration::readMqttCredentials() {
	byte len;
	unsigned addr = EEPROM_MQTT;
	char* buf;

	for (int i = 0; i < 4; i++) {
		mqttCreds.ServerIP[i] = eepromRead(addr);
		addr++;
	}

	mqttCreds.Port = eepromRead2(addr);
	addr += 2;

	len = eepromRead(addr);
	addr += 1;
	buf = (char*)malloc(len + 1);

	for (int i = 0; i < len; i++) {
		buf[i] = eepromRead(addr);
		addr++;
	}
	buf[len] = 0;
	mqttCreds.Root = String(buf);
	free(buf);


	len = eepromRead(addr);
	addr += 1;
	if (len != 0) {
		buf = (char*)malloc(len + 1);

		for (int i = 0; i < len; i++) {
			buf[i] = eepromRead(addr);
			addr++;
		}
		buf[len] = 0;
		mqttCreds.Login = String(buf);
		free(buf);
	}
	else {
		mqttCreds.Login = "";
	}
	len = eepromRead(addr);
	addr += 1;
	if (len != 0) {
		buf = (char*)malloc(len + 1);

		for (int i = 0; i < len; i++) {
			buf[i] = eepromRead(addr);
			addr++;
		}
		buf[len] = 0;
		mqttCreds.Password = String(buf);
		free(buf);

		mqttCreds.Password[len] = 0;
	}
	else {
		mqttCreds.Password = "";
	}
	
		Debug("EEPROM:MQTT:URL:" +  PrintIP(mqttCreds.ServerIP));
		Debug("EEPROM:MQTT:Port:" + String(mqttCreds.Port));
		Debug("EEPROM:MQTT:Root:" + String(mqttCreds.Root));
		Debug("EEPROM:MQTT:Login:" + String(mqttCreds.Login));
		Debug("EEPROM:MQTT:Password:" + String(mqttCreds.Password));
		
}


void Configuration::writeMqttCredentials() {
	byte len;
	unsigned addr = EEPROM_MQTT;

	for (int i = 0; i < 4; i++) {
		eepromWrite(addr, mqttCreds.ServerIP[i]);
		addr++;
	}

	eepromWrite(addr, (unsigned)mqttCreds.Port);
	addr += 2;

	len = mqttCreds.Root.length();
	eepromWrite(addr, len);
	addr += 1;
	for (int i = 0; i < len; i++) {
		eepromWrite(addr, (byte)mqttCreds.Root[i]);
		addr++;
	}

	len = mqttCreds.Login.length();
	eepromWrite(addr, len);
	addr += 1;
	if (len != 0) {
		for (int i = 0; i < len; i++) {
			eepromWrite(addr, (byte)mqttCreds.Login[i]);
			addr++;
		}
	}

	len = mqttCreds.Password.length();
	eepromWrite(addr, len);
	addr += 1;
	if (len != 0) {
		for (int i = 0; i < len; i++) {
			eepromWrite(addr, (byte)mqttCreds.Password[i]);
			addr++;
		}
	}
}


String Configuration::PrintIP(IPAddress addr) {
	return String(addr[0]) + "." + String(addr[1]) + "." + String(addr[2]) + "." + String(addr[3]);
}

void Configuration::ethernetSetup() {
	
	mac[5] = boardId;

	if (Ethernet.begin(mac) == 0) {
		Debug("Failed to configure Ethernet using DHCP");
		// no point in carrying on, so do nothing forevermore:
		// try to congifure using IP address instead of DHCP:
		IPAddress ip(192, 168, 0, 101);
		Ethernet.begin(mac, ip);
	}
	//Debug("Point2");
	Debug("Server is at: " +PrintIP(Ethernet.localIP()));
}