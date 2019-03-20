/*
 Name:		ThermoBus.ino
 Created:	20.03.19 12:34:11
 Author:	Igor Shevchenko
*/

// the setup function runs once when you press reset or power the board
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS	2

OneWire bus(ONE_WIRE_BUS);
DallasTemperature thermometers(&bus);

DeviceAddress *addrThermometers;
int tCnt = 0;
unsigned long lastLoop = millis();

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		// zero pad the address if necessary
		if (deviceAddress[i] < 16) Serial.print("0");
		Serial.print(deviceAddress[i], HEX);
	}
}



void setup() {

	Serial.begin(115200);
	Serial.println("Start thermometer testing");

	thermometers.begin();
	Serial.print("Locating devices...");
	Serial.print("Found ");
	tCnt = thermometers.getDS18Count();
	Serial.print(tCnt, DEC);
	Serial.println(" devices.");

	// report parasite power requirements
	Serial.print("Parasite power is: ");
	if (thermometers.isParasitePowerMode()) Serial.println("ON");
	else Serial.println("OFF");

	// Iterate devices
	addrThermometers = new DeviceAddress[tCnt];

	for (int i = 0; i < tCnt; i++) {
		bool res = thermometers.getAddress(addrThermometers[i], i);
		if (!res) {
			Serial.println("The thermometer:" + String(i) + " not found!");
		}
		else {
			Serial.print("Thermometer ");
			printAddress(addrThermometers[i]);
			Serial.println(" Found!");
		}
	}

	thermometers.setResolution(10);
	thermometers.requestTemperatures();
}

// the loop function runs over and over again until power down or reset
void loop() {
	//Serial.println("Loop:" + String(millis()) + "#" + String(lastLoop));
	if (millis() - lastLoop >= 1000) {
		lastLoop = millis();
		Serial.println("Temperatures:");
		for (int i = 0; i < tCnt; i++) {
			printAddress(addrThermometers[i]);
			Serial.println(" = " + String(thermometers.getTempC(addrThermometers[i]),4));
		}
		thermometers.requestTemperatures();
	}
}
