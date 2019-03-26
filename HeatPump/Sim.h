// Sim.h
#pragma once
#ifndef _SIM_h
#define _SIM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Simulator.h"

const byte ROWS0 = 4; // 4 строки
const byte COLS0 = 4; // 4 столбца
const byte PINS0 = 14; // number of sensors
const byte CASES0 = 4;
byte rowKeyPadPins0[ROWS0] = { 14, 15, 16, 17 };
byte colKeyPadPins0[COLS0] = { 18, 19, 20, 21 };
//const byte pinMap0[PINS0] = { 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 8, 99/*,38,40,42,44*/};
const byte pinMap0[PINS0] = { 61, 62, 63, 64, 65, 66, 67, 8, 99/*,38,40,42,44*/ };


extern Simulator* sim;

//void initSim();

#endif

