#include "Configuration.h"
#include "EEPROM.h"

extern Simulator* sim;


Configuration::Configuration()
{
}



void Configuration::loop() {
	DevMgr.loop();
	ScriptMgr.loop();
	if (IsMqtt()) {
		mqttClient->MqttLoop();
	}
	if (IsWeb()) {
		web->loop();
	}
}

void Configuration::setup() {

	readEepromInfo();

	if (availEthernet) {
		ethernetSetup();
		isEthernetReady = true;
	}

	DevMgr.begin();


	if (IsEthernet() && availMqttClient) {
		mqttClient = new Mqtt();
		isMqttReady = mqttClient->setup();
		//Loger::Debug("Point10");
		Loger::Debug("Is MQTT ready?" + String(isMqttReady));
		mqttClient->InitialActions();
	}
	Loger::Debug("Starting script Manager");
	isReady = ScriptMgr.setup();
	Loger::Debug("Is Config Ready?" + String(isReady));
	
	if (IsEthernet() && availWebServer) {
		Loger::Debug("Web Server Is Starting...");
		web = new ArduinoServer();

		isWebReady = web->setup();
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

void Configuration::eepromWrite(unsigned int addr, byte value[8]) {

	for (int i = 0; i < 8; i++) {
		EEPROM.write(addr + i, value[i]);
	}
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
		if (IsMqtt()) {
			mqttClient->PublishDesiredTemp(desiredTemp);
		}
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

	Loger::Debug("BoardId=" + String(boardId)+"; BoardName=" + BoardName());
	//Read Temperature's sensor Addresses

	for (int i = 0; i < DevMgr.getNumberTemp(); i++) {
		for (int j = 0; j < 8; j++) {
			DevMgr.tempSensors[i].DevAddress[j] = eepromRead(EEPROM_T_ADDRESS + i * 8 + j);

		}
		//PrintMac(DevMgr.tempSensors[i].DevAddress);
	}

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
	
	Loger::Debug("EEPROM:MQTT:URL:" +  PrintIP(mqttCreds.ServerIP));
	Loger::Debug("EEPROM:MQTT:Port:" + String(mqttCreds.Port));
	Loger::Debug("EEPROM:MQTT:Root:" + String(mqttCreds.Root));
	Loger::Debug("EEPROM:MQTT:Login:" + String(mqttCreds.Login));
	Loger::Debug("EEPROM:MQTT:Password:" + String(mqttCreds.Password));
	
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
		Loger::Debug("Failed to configure Ethernet using DHCP");
		// no point in carrying on, so do nothing forevermore:
		// try to congifure using IP address instead of DHCP:
		IPAddress ip(192, 168, 0, 101);
		Ethernet.begin(mac, ip);
	}
	//Debug("Point2");
	Loger::Debug("Server is at: " +PrintIP(Ethernet.localIP()));
}

String Configuration::PrintMac(byte* devAddr, int format) {
	String res = "";
	
	if (devAddr != NULL) {
		for (int i = 0; i < 8; i++) {
			if (devAddr[i] < 16) {
				res += '0';
			}
			res += String(devAddr[i], HEX);
			if (format == 1) {
				if (i != 7) {
					res += ' ';
				}
			}
		}
	}
	Loger::Debug("MAC:" + res);
	
	//res = "QWERT";
	return res;
}