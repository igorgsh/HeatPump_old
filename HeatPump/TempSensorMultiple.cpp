// 
// 
// 

#include "TempSensorMultiple.h"

TempSensorMultiple::TempSensorMultiple(String label, int pin) {
	bus = new OneWire(pin);
	dt = new DallasTemperature(bus);
}

TempSensorMultiple::~TempSensorMultiple() {
	
	delete dt;
	delete bus;
}

bool TempSensorMultiple::begin() {
	bool ret = false;
	
	dt->begin();
	numberOfSensors = dt->getDS18Count();
	if (numberOfSensors > 0) {
		sensors = new TempSensor[numberOfSensors];
	}
}
