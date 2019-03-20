#include "DeviceManager.h"
#include "Loger.h"


DeviceManager::DeviceManager()
{
	TempDrv = new TemperatureBus(PIN_TEMPERATURE_BUS);
}


DeviceManager::~DeviceManager()
{
	delete TempDrv;
}

void DeviceManager::begin() {

	TempDrv->begin();

	for (int i = 0; i < getNumberTemp(); i++) {
		tempSensors[i].begin();
	}
	for (int i = 0; i < getNumberPump(); i++) {
		pumps[i].begin();
	}
	for (int i = 0; i < getNumberCont(); i++) {
		contacts[i].begin();
	}

	compressor.begin();
}


void DeviceManager::loop() {
	//check all thermometers
	for (int i = 0; i < getNumberTemp(); i++) {
		tempSensors[i].loop();
	}
	TempDrv->loop();
	
	//check all contactors
	for (int i = 0; i < getNumberCont(); i++) {
		contacts[i].loop();
	}
	/*
	for (int i = 0; i < getNumberTemp(); i++) {
	Debug2_("Sensor:",tempSensors[i].getLabel());
	Debug2_(";Temperature=", String(tempSensors[i].getValue()));
	Debug2(";Status:", String(tempSensors[i].getActionStatus()));
	}
	Debug("====");
	*/

}

