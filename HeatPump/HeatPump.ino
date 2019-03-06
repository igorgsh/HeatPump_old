/*
 Name:		HeatPump.ino
 Created:	14.05.17 11:26:52
 Author:	Igor Shevchenko

 Heat Pump controller
*/

#include <PubSubClient.h>
//#include <EEPROM.h>
#include <Keypad.h>
#include <Key.h>



//#include <OneWire.h>
//#include <DallasTemperature.h>
//#include <EEPROM.h>
//#include <EthernetUdp.h>
//#include <EthernetServer.h>
//#include <EthernetClient.h>
//#include <Ethernet.h>
//#include <Dns.h>
//#include <Dhcp.h>
//#include <SD.h>
#include "Configuration.h"
#include <MsTimer2.h>
#include "Definitions.h"
#include "Configuration.h"
#include "Simulator.h"



#define LED_PIN	13
#include "AutoTests.h"

Simulator* sim;
AutoTests* test;


extern void initSim();

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
	Loger::Debug("Begin 1.0");
	Config.begin();

	Loger::Debug("Point 1");

	if (Config.IsSimulator()) {
		initSim();
		sim->SetCaseNumber(0);
	}
	Loger::Debug("Point 4");

	if (Config.IsAutoTesting()) {
		test = new AutoTests();
	}
	// Initialize configuration
	Loger::Debug("Point 5");

	//Set a timer 
	MsTimer2::set(100, Timer2);
	MsTimer2::start();
	Loger::Debug("Start!!!");
	Config.isHardwareReady = Config.ScriptMgr.setup();
	isReady = true;
	//	delay(500);
	Loger::Debug("Setup is over#" + String(Config.isHardwareReady) + "#" + String(isReady));
	// Prepare the light indicator 
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);

}

// the loop function runs over and over again until power down or reset
void loop() {
	if (Config.IsSimulator()) {
		sim->loop();
	}
	Config.loop();
	if (Config.IsAutoTesting()) {
		test->loop();
	}

}
