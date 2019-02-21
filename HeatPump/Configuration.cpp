#include "Configuration.h"
#include "Definitions.h"
#include "EEPROM.h"

//#define WEB_ENABLED


Configuration::Configuration()
{
}


Configuration::~Configuration()
{
}

void Configuration::loop() {
	DevMgr.loop();
	if (isHardwareReady) {
		ScriptMgr.loop();
	}
#ifdef WEB_ENABLED
	web.loop();
#endif // WEB_ENABLED
}

void Configuration::begin() {
	DevMgr.begin();

#ifdef WEB_ENABLED
	Debug("Server Is Starting...");
	web.begin();
#endif
}

float Configuration::OutTemperature() {
	float outTemp = 0;
	if (desiredTemp <= 20)
		outTemp = 25;
	else if (desiredTemp >= 45)
		outTemp = 45;
	else
		outTemp = desiredTemp + TEMPERATURE_DELTA;
	return outTemp;
}

void Configuration::EepromWrite(unsigned int addr, byte value) {

	EEPROM.write(addr, value);
}

void Configuration::EepromWrite(unsigned int addr, unsigned int value) {

	EEPROM.write(addr, value % 0xFF);
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

byte Configuration::getDesiredTemp() {
#ifdef _SIMULATOR_
	return (byte)(sim->GetIntResult(99));
#else
	return desiredTemp;
#endif // _SIMULATOR_

}
