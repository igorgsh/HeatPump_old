/*
 Name:		HeatPump.ino
 Created:	14.05.17 11:26:52
 Author:	Igor Shevchenko

 Heat Pump controller
*/
#include <Keypad.h>
#include <Key.h>

//#include <OneWire.h>
//#include <DallasTemperature.h>
//#include <EEPROM.h>
#include <EthernetUdp.h>
#include <EthernetServer.h>
#include <EthernetClient.h>
#include <Ethernet.h>
#include <Dns.h>
#include <Dhcp.h>
#include <SD.h>
#include "Configuration.h"
#include <MsTimer2.h>
#include "Definitions.h"
#include "Configuration.h"
#include "Simulator.h"

DebugLevel dLevel = D_ALL;

#define WEB_ENABLED

#define SDCARD_SS	4
#define LED_PIN	13
//#define _SIMULATOR_
#include "Sim.h"
#include "AutoTests.h"

#ifdef _SIMULATOR_
Simulator* sim;
#endif

#ifdef _AUTO_TESTING_
AutoTests* test;
#endif

Configuration Config;

bool isReady = false;

// the setup function runs once when you press reset or power the board

void Timer2() { //it is started every 100ms
	if (isReady) {
		Config.counter100++;
		if (Config.counter100 % 5 == 0) {
			Config.counter500++;
		}
		if (Config.counter100 % 10 == 0) {
			Config.counter1s++;
		}
	}
}



void setup() {
	//Configure Serial port and SD card
	Serial.begin(115200);
	randomSeed(analogRead(0));
	Debug("Begin 1.0");

	#ifdef _SIMULATOR_
	initSim();
	sim->SetCaseNumber(0);
	#endif
#ifdef _AUTO_TESTING_
	test = new AutoTests();

#endif // _AUTO_TESTING_

	SD.begin(SDCARD_SS);

	// Initialize configuration
	Config.begin();

	// Prepare the light indicator 
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);
	//Set a timer 
	MsTimer2::set(100, Timer2);
	MsTimer2::start();
	Debug("Start!!!");
	isReady=true;
	//	delay(500);

}

// the loop function runs over and over again until power down or reset
void loop() {
#ifdef _SIMULATOR_
	sim->loop();
#endif
	Config.loop();
#ifdef _AUTO_TESTING_
	test->loop();
#endif //_AUTO_TESTING_

}
