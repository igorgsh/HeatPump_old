#include "DeviceManager.h"
#include "Definitions.h"


DeviceManager::DeviceManager()
{
}


DeviceManager::~DeviceManager()
{
}

void DeviceManager::begin() {
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

void DeviceManager::loop1() {
	//check all thermometers
	for (int i = 0; i < getNumberTemp(); i++) {
		tempSensors[i].loop();
	}
	//check all contactors
	for (int i = 0; i < getNumberCont(); i++) {
		contacts[i].loop();
	}
}

void DeviceManager::loop5() {

}


void DeviceManager::loop10() {
	/*
	for (int i = 0; i < getNumberTemp(); i++) {
		Debug2_("Sensor:",tempSensors[i].getLabel());
		Debug2_(";Temperature=", String(tempSensors[i].getValue()));
		Debug2(";Status:", String(tempSensors[i].getActionStatus()));
	}
	Debug("====");
	*/
}
