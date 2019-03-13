#pragma once
#include <Arduino.h>

typedef enum {
	UT_NoUnit = 0,
	UT_Thermometer = 1,
	UT_Contactor = 2,
	UT_Pump_Geo = 3,
	UT_Pump_C1 = 4,
	UT_Pump_C2 = 5,
	UT_Pump_F1 = 6,
	UT_Pump_F2 = 7,
	UT_Compressor = 8
} UnitType;

class Unit
{
public:
	Unit(String label, UnitType unitType);
	~Unit();
	//Label of Sensor
	String GetLabel() { return label; };
	void SetLabel(String lbl) { label = lbl; };
	UnitType GetType() { return type; };

private:
	UnitType type = UnitType::UT_NoUnit;
	String label;

};

