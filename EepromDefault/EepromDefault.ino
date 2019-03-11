/*
 Name:		EepromDefault.ino
 Created:	04.03.19 15:48:09
 Author:	Igor Shevchenko
*/

// the setup function runs once when you press reset or power the board
#include <EEPROM.h>
#include <IPAddress.h>


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
	unsigned int addr = 0x14;
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
	unsigned int addr = 0x14;

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


void setup() {

	Serial.begin(115200);

	// Board ID
	EEPROM.write(0, 0x07);
	// Desired temp 25 degree
	EEPROM.write(1, 25 * 2);

	//Mqtt Creds
	IPAddress ip(192, 168, 0, 99);
	WriteMqttCredentials(1883, ip, "HeatPump", "", "");

	byte b;

	b = EEPROM.read(0);
	Serial.println("Id#" + String(b));
	b = EEPROM.read(1);
	Serial.println("Temp#" + String((float)b / 2));

	ReadMqttCredentials();

	Serial.println("Done!");
}

// the loop function runs over and over again until power down or reset
void loop() {

}
