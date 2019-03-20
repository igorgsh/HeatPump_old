#include "TempSensor.h"



TempSensor::TempSensor(String label, TemperatureDriver* drv, float lowerRange, float upperRange) : Sensor(label, -1, UnitType::UT_Thermometer, lowerRange, upperRange)
{
	driver = drv;
	for (int i = 0; i < 8; i++) {
		DevAddress[i] = 0;
	}
	DevAvailable = false;
}

TempSensor::~TempSensor()
{
}

bool TempSensor::loop() {
	bool ret = false;

}
