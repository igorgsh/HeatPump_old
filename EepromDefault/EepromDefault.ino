/*
 Name:		EepromDefault.ino
 Created:	04.03.19 15:48:09
 Author:	Igor Shevchenko
*/

// the setup function runs once when you press reset or power the board
#include <EEPROM.h>
#include <IPAddress.h>

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

void EepromWrite(unsigned int addr, byte value[8]) {

	for (int i = 0; i < 8; i++) {
		EEPROM.write(addr + i, value[i]);
	}
}


void EepromWrite(unsigned int addr, byte value) {

	EEPROM.write(addr, value);
}

void EepromWrite(unsigned int addr, unsigned int value) {

	//Serial.println("value=" + String(value));
	EEPROM.write(addr, value & 0xFF);
	//Serial.println("value1=" + String(value & 0xFF));

	EEPROM.write(addr + 1, (value >> 8) & 0xFF);
	//Serial.println("value2=" + String((value >> 8) & 0xFF));

	//Serial.println(String(value & 0xFF) + ":" + String((value >> 8) & 0xFF));
}

byte EepromRead(unsigned int addr) {
	return EEPROM.read(addr);
}

unsigned int EepromRead2(unsigned int addr) {
	byte x;
	byte b;

	x = EEPROM.read(addr);
	b = EEPROM.read(addr + 1);
	//Serial.println("x=" + String(x) + ";b=" + String(b));
	//Serial.println(String(((unsigned int)b << 8) | x));
	return (unsigned)b << 8 | x;
}


void ReadMqttCredentials() {
	byte len;
	unsigned int addr = EEPROM_MQTT;
	String str0;
	char* buf;

	Serial.print("IP#");
	for (int i = 0; i < 4; i++) {
		byte b = EepromRead(addr);
		Serial.print(b);
		Serial.print(".");
		addr += 1;
	}
	Serial.println();

	unsigned int port = EepromRead2(addr);
	Serial.println("Port#" + String(port));
	addr += 2;

	len = EepromRead(addr);
	buf = (char*)malloc(len + 1);

	Serial.print("Root#" + String(len) + ":");
	addr += 1;

	for (int i = 0; i < len; i++) {
		buf[i] = EepromRead(addr);
		addr++;
	}
	buf[len] = 0;
	str0 = String(buf);
	free(buf);
	Serial.println(str0 + "#");


	len = EepromRead(addr);
	Serial.print("Login#" + String(len) + ":");
	addr += 1;
	str0 = "";

	if (len != 0) {
		buf = (char*)malloc(len + 1);

		for (int i = 0; i < len; i++) {
			buf[i] = EepromRead(addr);
			addr++;
		}
		buf[len] = 0;
		str0 = String(buf);
		free(buf);

	}
	Serial.println(str0 + "#");


	len = EepromRead(addr);
	Serial.print("Pwd#" + String(len) + ":");
	str0 = "";
	addr += 1;

	if (len != 0) {

		buf = (char*)malloc(len + 1);
		for (int i = 0; i < len; i++) {
			buf[i] = EepromRead(addr);
			addr++;
		}
		buf[len] = 0;
		str0 = String(buf);
		free(buf);

	}
	Serial.println(str0 + "#");


}



void WriteMqttCredentials(unsigned int port, IPAddress ip, String root, String login, String password) {
	byte len;
	unsigned int addr = EEPROM_MQTT;

	// URL
	for (int i = 0; i < 4; i++) {
		EepromWrite(addr, ip[i]);
		addr++;
	}
	// port
	EepromWrite(addr, (unsigned int)port);
	addr += 2;

	// root
	len = root.length();
	EepromWrite(addr, len);
	addr += 1;
	for (int i = 0; i < len; i++) {
		EepromWrite(addr, (byte)(root[i]));
		addr++;
	}

	// login
	len = login.length();
	EepromWrite(addr, len);
	addr += 1;
	if (len != 0) {
		for (int i = 0; i < len; i++) {
			EepromWrite(addr, (byte)(login[i]));
			addr++;
		}
	}

	// password
	len = password.length();
	EepromWrite(addr, len);
	addr += 1;
	if (len != 0) {
		for (int i = 0; i < len; i++) {
			EepromWrite(addr, (byte)(password[i]));
			addr++;
		}
	}
}

void ReadTAddresses() {
	Serial.println("Thermometer addresses:");
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 8; j++) {
			Serial.print(String(EepromRead(EEPROM_T_ADDRESS + i * 8 + j), HEX));
			if (i != 7) {
				Serial.print(":");
			}
		}
		Serial.println();
	}
}


void setup() {

	Serial.begin(115200);

	// Board ID
	EEPROM.write(EEPROM_ID, 0x07);
	// Desired temp 25 degree
	EEPROM.write(EEPROM_DESIRED_TEMP, 25 * 2);

	//Thermometer Addresses
	byte tAddr[7][8] = {
		{0x28, 0x4F, 0x32,0x34,0x08,0x00,0x00,0x2B},
		{0x28,0xFF,0x40,0xA8,0xB4,0x16,0x03,0x92},
		{0x28,0xFF,0x08,0x96,0x90,0x16,0x04,0x91},
		{0x28,0xFF,0x68,0xE5,0x90,0x16,0x04,0xA0},
		{0x28,0xFF,0xFC,0x90,0x90,0x16,0x04,0x21},
		{0x28,0xFF,0x72,0x9C,0x90,0x16,0x04,0x5A},
		{0x28,0xFF,0x25,0xF3,0x87,0x16,0x03,0x27}
	};

	for (int i = 0; i < 7; i++) {
		EepromWrite(EEPROM_T_ADDRESS + i * 8, tAddr[i]);
	}

	//Mqtt Creds
	IPAddress ip(192, 168, 0, 99);
	WriteMqttCredentials(1883, ip, "Equipment/HeatPump", "", "");

	byte b;

	b = EEPROM.read(0);
	Serial.println("Id#" + String(b));
	b = EEPROM.read(1);
	Serial.println("Temp#" + String((float)b / 2));

	ReadTAddresses();
	ReadMqttCredentials();

	Serial.println("Done!");
}

// the loop function runs over and over again until power down or reset
void loop() {

}
