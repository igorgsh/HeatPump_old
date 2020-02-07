#pragma once

//#include "TempSensorSingle.h"
#include "Pump.h"
#include "Compressor.h"
#include "Contactor.h"
#include "TempSensor.h"
#include "TemperatureDriver.h"
#include "TemperatureBus.h"

#define NUMBER_OF_TEMP	7
#define NUMBER_OF_CONTACTOR	1
#define NUMBER_OF_PUMP	3

#define PIN_TEMPERATURE_BUS 2

class DeviceManager
{
public:
	DeviceManager();
	~DeviceManager();

	void begin();
	//TemperatureBus* TempDrv;
	TemperatureBus TempDrv = TemperatureBus(PIN_TEMPERATURE_BUS);

	TempSensor tempSensors[NUMBER_OF_TEMP] = {
		/* 0*/	TempSensor("Geo_In",1 ,-20.0, 50.0),	//T1
		/* 1*/	TempSensor("Geo_Out",2 ,-20.0, 50.0),	//T2
		/* 2*/	TempSensor("Cond_In",3,-30.0,50.0),	//T3
		/* 3*/	TempSensor("Cond_Out",4,-30.0,50.0),	//T4
		/* 4*/	TempSensor("Compressor",5, -30.0, 120.0),	//T5
		/* 5*/	TempSensor("HP_In",6,-30.0,110.0),	//T6
		/* 6*/	TempSensor("HP_Out",7,-30.0, 110.0)	//T7
	};

	//TempSensorSingle tempSensors[NUMBER_OF_TEMP] = {
	//	/* 0*/	TempSensorSingle("GeoI",	22,-20.0, 50.0),	//T1
	//	/* 1*/	TempSensorSingle("GeoO",	23,-20.0, 50.0),	//T2
	//	/* 2*/	TempSensorSingle("EvoI",	24,-30.0, 50.0),	//T3 = T6
	//	/* 3*/	TempSensorSingle("EvoO",	25,-30.0,50.0),	//T4
	//	/* 4*/	TempSensorSingle("CondI",	26,-30.0, 110.0),	//T5
	//	/* 5*/	TempSensorSingle("CondO",	27,-30.0,50.0),	//T6 = T3
	//	/* 6*/	TempSensorSingle("HpO",	28,-30.0, 110.0),	//T7
	//	/* 7*/	TempSensorSingle("HpI",	29,-30.0, 110.0),	//T8
	//	/* 8*/	TempSensorSingle("Comp",	30,-30.0,120.0),//T9
	//	/* 9*/	TempSensorSingle("Unkn",	31,-50.0,50.0),	//T10
	//	/*10*/	TempSensorSingle("Room",	32,-50.0,50.0),	//T11
	//	/*11*/	TempSensorSingle("Street",33,-50.0,50.0)	//T12
	//};

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
	TempSensor* tCondI = &tempSensors[2];
	TempSensor* tCondO = &tempSensors[3];
	TempSensor* tCompr = &tempSensors[4];
	TempSensor* tHpI = &tempSensors[5];
	TempSensor* tHpO = &tempSensors[6];

	TempSensor* currentTemp = &tempSensors[3];

	/*
	TempSensor* tGeoI = &tempSensors[0];
	TempSensor* tGeoO = &tempSensors[1];
	TempSensorSingle* tEvoI = &tempSensors[2];
	TempSensorSingle* tEvoO = &tempSensors[3];
	TempSensorSingle* tCondI = &tempSensors[4];
	TempSensorSingle* tCondO = &tempSensors[5];
	TempSensorSingle* tHpO = &tempSensors[6];
	TempSensorSingle* tHpI = &tempSensors[7];
	TempSensorSingle* tComp = &tempSensors[8];
	TempSensorSingle* tUnkn = &tempSensors[9];
	TempSensorSingle* currentTemp = &tempSensors[10];
	TempSensorSingle* tStreet = &tempSensors[11];
*/

	Contactor contacts[NUMBER_OF_CONTACTOR] = { Contactor("Flow", 8, HIGH, LOW) };
	
	Contactor* cFlow = &contacts[0];

	int getNumberTemp() { return NUMBER_OF_TEMP; }
	int getNumberCont() { return NUMBER_OF_CONTACTOR; }
	int getNumberPump() { return NUMBER_OF_PUMP; }
	void loop();
};

