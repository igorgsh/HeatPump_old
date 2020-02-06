/*
 Name:		HeatPump.ino
 Created:	14.05.17 11:26:52
 Author:	Igor Shevchenko

 Heat Pump controller
*/
//#include <PubSubClient.h>
//#include <Keyboard.h>
//#include <DallasTemperature.h>
//#include "TemperatureBus.h"
#include <FlexiTimer2.h>
#include <Ethernet.h>
//#include <Keypad.h>
//#include <Key.h>

#include <SD.h>
#include "Configuration.h"
//#include <MsTimer2.h>

#include "Configuration.h"
#include "Simulator.h"
#include "AutoTests.h"
#include "Loger.h"

DebugLevel dLevel = D_ALL;


#define LED_PIN	13

Simulator* sim;
AutoTests* test;

extern void initSim();

Configuration Config;


void Timer2() { //it is started every 100ms
	//Loger::Debug("Timer!!!");
	if (Config.IsReady()) {
		Config.Counter100++;
		if (Config.Counter100 % 5 == 0) {
			Config.Counter500++;
		}
		if (Config.Counter100 % 10 == 0) {
			Config.Counter1s++;
			//Loger::Debug("Counter1s tick");
		}
	}
}



void setup() {
	//Configure Serial port and SD card
	Serial.begin(115200);
	randomSeed(analogRead(0));
	Loger::Debug("Begin 1.0");

	// Prepare the light indicator 
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);
	//Set a timer 
//	MsTimer2::set(100, Timer2);
//	MsTimer2::start();
	FlexiTimer2::set(100, Timer2);
	FlexiTimer2::start();

	if (Config.IsSimulator()) {
		initSim();
		sim->SetCaseNumber(0);
	}
	if (Config.IsAutoTesting()) {
		test = new AutoTests();
	}

//	SD.begin(SDCARD_SS);

	Config.setup();

	Loger::Debug("Setup is over#" + String(Config.IsReady()));
	digitalWrite(LED_PIN, HIGH);

}

// the loop function runs over and over again until power down or reset
void loop() {
	//Loger::Debug("Config100=" + String(Config.Counter100));
	if (Config.IsSimulator()) {
		sim->loop();
	}
	Config.loop();

	if (Config.IsAutoTesting()) {
		test->loop();
	}
}
