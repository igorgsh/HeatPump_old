// 
// 
// 

#include "TemperatureBus.h"
#include "Configuration.h"

extern Configuration Config;

TemperatureBus::TemperatureBus(int pin) {
	bus = new OneWire(pin);
	dt = new DallasTemperature(bus);
}

TemperatureBus::~TemperatureBus() {
	delete knownDevAvail;
	delete knownDevs;
	delete dt;
	delete bus;
}

bool TemperatureBus::begin() {
	bool ret = false;
	
	sensors = Config.DevMgr.tempSensors;
	dt->begin();
	numberOfSensors = dt->getDS18Count();
	knownDevs = new DeviceAddress[numberOfSensors];
	knownDevAvail = new bool[numberOfSensors];
	for (int i = 0; i < numberOfSensors; i++) {
		dt->getAddress(knownDevs[i], i);
		knownDevAvail[i] = false;
		for (int j = 0; j < Config.DevMgr.getNumberTemp; j++) {
			if (compareAddresses(sensors[j].DevAddress, knownDevs[i])) {
				sensors[j].DevAvailable = true;
				knownDevAvail[i] = true;
			}
		}
	}

	dt->setResolution(DT_DEFAULT_RESOLUTION);
	dt->requestTemperatures();
}


bool TemperatureBus::compareAddresses(DeviceAddress addr1, DeviceAddress addr2) {
	bool ret = true;

	for (int i = 0; i < 8; i++) {
		ret &= (addr1[i] == addr2[i]);
	}

	return ret;
}
