#include "Configuration.h"
#include "Definitions.h"
#include "EEPROM.h"

#define WEB_ENABLED 1


Configuration::Configuration()
{
}


Configuration::~Configuration()
{
}

void Configuration::loop1() {
	//Debug("Loop1");
	DevMgr.loop1(counter1);
	ScenMgr.loop1(counter1);
}

void Configuration::loop5() {
	//Debug("Loop5");
#ifdef WEB_ENABLED
	web.loop();

#endif // WEB_ENABLED

	DevMgr.loop5(counter5);
	ScenMgr.loop5(counter5);
}

void Configuration::loop10() {
	//Debug("Loop10");
	DevMgr.loop10(counter10);
	ScenMgr.loop10(counter10);
}

void Configuration::begin() {
	DevMgr.begin();
	//ScenMgr.begin();

#ifdef WEB_ENABLED
	Debug("Server Is Starting...");
	web.begin();
#endif
}

float Configuration::OutTemperature() {
	float outTemp = 0;
	if (desiredTemp< 20)
		outTemp = 25;
	else if (desiredTemp < 25)
		outTemp = 30;
	else if (desiredTemp < 30)
		outTemp = 35;
	else if (desiredTemp < 35)
		outTemp = 40;
	else 
		outTemp = 45;
	return outTemp;
}

void Configuration::EepromWrite(unsigned int addr, byte value) {

	EEPROM.write(addr, value);
}

void Configuration::EepromWrite(unsigned int addr, unsigned int value) {

	EEPROM.write(addr, value%256);
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
	return (unsigned)b << 8 & x;
}


void Configuration::setDesiredTemp(byte value) {
	
	if (desiredTemp != value) { //optimization: reduce number of write to EEPROM
		desiredTemp = value;
		EepromWrite(EEPROM_Desired_Temp, desiredTemp);
	}
}