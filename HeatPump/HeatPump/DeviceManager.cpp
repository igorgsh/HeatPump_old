#include "DeviceManager.h"



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
}

void DeviceManager::loop1(unsigned long counter) {
	for (int i = 0; i < getNumberTemp(); i++) {
		tempSensors[i].loop(counter);
	}
}

void DeviceManager::loop5(unsigned long counter) {
}


void DeviceManager::loop10(unsigned long counter) {
}
