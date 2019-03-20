#include "DT.h"



DT::~DT()
{
	free(addresses);
}

void DT::begin() {
	DallasTemperature::begin();
	addresses = (DeviceAddress*)malloc(sizeof(DeviceAddress) * getDeviceCount());
	for (int i = 0; i < getDeviceCount(); i++) {
		getAddress(addresses[i], i);
	}
	setCheckForConversion(false);
	setWaitForConversion(false);
}
/*
bool DT::getAddress(uint8_t* address, uint8_t index) {
	if (index >= 0 && index < getDeviceCount()) {
		address = addresses[index];
		return true;
	}
	else {
		return false;
	}
}
*/
/*
bool DT::isConversionAvailable(int index) {
	if (index >= 0 && index < getDeviceCount()) {
		DallasTemperature::isConversionComplete()
		return DallasTemperature:: isConversionAvailable(addresses[index]);

	}
	else {
		return false;
	}

}
*/