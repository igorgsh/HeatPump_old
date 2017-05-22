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
	compressor.begin();
}

void DeviceManager::loop1(unsigned long counter) {
	//check all thermometers
	for (int i = 0; i < getNumberTemp(); i++) {
		tempSensors[i].loop(counter);
	}
}

void DeviceManager::loop5(unsigned long counter) {

}


void DeviceManager::loop10(unsigned long counter) {
	for (int i = 0; i < getNumberTemp(); i++) {
		Debug2_("Sensor:",tempSensors[i].getLabel());
		Debug2_(";Temperature=", tempSensors[i].getValue());
		Debug2(";Status:", tempSensors[i].getActionStatus());
	}

}
