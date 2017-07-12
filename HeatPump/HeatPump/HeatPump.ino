/*
 Name:		HeatPump.ino
 Created:	14.05.17 11:26:52
 Author:	Igor Shevchenko

 Heat Pump controller
*/

//#include <OneWire.h>
//#include <DallasTemperature.h>
#include <EEPROM.h>
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

DebugLevel dLevel = D_ALL;

#define WEB_ENABLED

#define SDCARD_SS	4
#define LED_PIN	13

Configuration Config;

// the setup function runs once when you press reset or power the board

void Timer2() { //it is started every 100ms
				//static byte counter100 = 0;

	Config.loop1();
	Config.counter1++;
	if (Config.counter1 % 5 == 0) {
		Config.loop5();
		Config.counter5++;
		if (Config.counter1 % 10 == 0) {
			Config.loop10();
			Config.counter10++;
		}
	}
}



void setup() {
	//Configure Serial port and SD card
	Serial.begin(115200);
	Debug("Begin");
	
	SD.begin(SDCARD_SS);

	// Initialize configuration
	Config.begin();

	// Prepare the light indicator 
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);

	/*
	// locate devices on the bus
	Serial.print("Locating devices...");
	Serial.print("Found ");
	Serial.print(Config.getNumberTemp() + Config.getNumberCont(), DEC);
	Serial.println(" devices.");
	*/

	//Set a timer 
	MsTimer2::set(100, Timer2);
	MsTimer2::start();
	Debug("Start!!!");
	//	delay(500);

}

// the loop function runs over and over again until power down or reset
void loop() {
  
}
