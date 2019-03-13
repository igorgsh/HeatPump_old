#pragma once

#include "TempSensor.h"
#include "Pump.h"
#include "Compressor.h"
#include "Contactor.h"

#define NUMBER_OF_TEMP	12
#define NUMBER_OF_CONTACTOR	1
#define NUMBER_OF_PUMP	3

class DeviceManager
{
public:
	DeviceManager();
	~DeviceManager();

	void begin();

	TempSensor tempSensors[NUMBER_OF_TEMP] = {
		/* 0*/	TempSensor("GeoI",	22,-20.0, 50.0),	//T1
		/* 1*/	TempSensor("GeoO",	23,-20.0, 50.0),	//T2
		/* 2*/	TempSensor("EvoI",	24,-30.0, 50.0),	//T3 = T6
		/* 3*/	TempSensor("EvoO",	25,-30.0,50.0),	//T4
		/* 4*/	TempSensor("CondI",	26,-30.0, 110.0),	//T5
		/* 5*/	TempSensor("CondO",	27,-30.0,50.0),	//T6 = T3
		/* 6*/	TempSensor("HpO",	28,-30.0, 110.0),	//T7
		/* 7*/	TempSensor("HpI",	29,-30.0, 110.0),	//T8
		/* 8*/	TempSensor("Comp",	30,-30.0,120.0),//T9
		/* 9*/	TempSensor("Unkn",	31,-50.0,50.0),	//T10
		/*10*/	TempSensor("Room",	32,-50.0,50.0),	//T11
		/*11*/	TempSensor("Street",33,-50.0,50.0)	//T12
	};

	Pump pumps[NUMBER_OF_PUMP] = { Pump("PGeo", UnitType::UT_Pump_Geo , 40,HIGH,1,1), //Geo Contour
									Pump("PC1", UnitType::UT_Pump_C1, 42,HIGH,1,1),	//Pump between HP and Tank
									Pump("PC2", UnitType::UT_Pump_C2, 44,HIGH,1,1), //Pump between Tank and Heaters
	};
	Compressor compressor = Compressor("PComp", 38, HIGH, 1, 1);

	Pump* pumpGeo = &pumps[0];
	Pump* pumpContour1 = &pumps[1];
	Pump* pumpContour2 = &pumps[2];

	TempSensor* tGeoI = &tempSensors[0];
	TempSensor* tGeoO = &tempSensors[1];
	TempSensor* tEvoI = &tempSensors[2];
	TempSensor* tEvoO = &tempSensors[3];
	TempSensor* tCondI = &tempSensors[4];
	TempSensor* tCondO = &tempSensors[5];
	TempSensor* tHpO = &tempSensors[6];
	TempSensor* tHpI = &tempSensors[7];
	TempSensor* tComp = &tempSensors[8];
	TempSensor* tUnkn = &tempSensors[9];
	TempSensor* currentTemp = &tempSensors[10];
	TempSensor* tStreet = &tempSensors[11];

	Contactor contacts[NUMBER_OF_CONTACTOR] = { Contactor("Flow", 8, HIGH, LOW) };
	
	Contactor* cFlow = &contacts[0];

	int getNumberTemp() { return NUMBER_OF_TEMP; }
	int getNumberCont() { return NUMBER_OF_CONTACTOR; }
	int getNumberPump() { return NUMBER_OF_PUMP; }
	void loop();
};

