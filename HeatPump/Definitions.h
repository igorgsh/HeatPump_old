#pragma once

typedef enum {
	D_OFF = 0,
	D_FATAL = 1,
	D_ERROR = 2,
	D_WARN = 3,
	D_INFO = 4,
	D_DEBUG = 5,
	D_ALL = 6
} DebugLevel;

extern DebugLevel dLevel;

#include "AutoTests.h"
#include "Simulator.h"

extern Simulator* sim;
extern AutoTests* test;

#define SerialLog2(LEVEL, X, Y) {if (LEVEL <= dLevel) {Serial.println((X),Y);}};
#define SerialLog2_(LEVEL, X, Y) {if (LEVEL <= dLevel) {Serial.print((X),Y);}};
#define SerialLog(LEVEL, X) {if (LEVEL <= dLevel) {Serial.println((X));}};
#define SerialLog_(LEVEL,X) {if (LEVEL <= dLevel) {Serial.print((X));}};
#define Debug(X)	SerialLog(D_DEBUG, X)
#define Debug_(X)	SerialLog_(D_DEBUG, X)
#define Debug2(X,Y)	SerialLog_(D_DEBUG,X); SerialLog(D_DEBUG, Y);
#define Debug2_(X,Y)	SerialLog_(D_DEBUG,X); SerialLog_(D_DEBUG, Y);
#define Debug3(X,Y,Z)	SerialLog_(D_DEBUG,X); SerialLog2(D_DEBUG, Y, Z);



/*
typedef enum {
	SCENARIO_NOCMD = 0,
	SCENARIO_STOP = 1,
	SCENARIO_START = 2,
	SCENARIO_FORCE_STOP = 3,
	SCENARIO_FORCE_START = 4
} ScenarioCmd;
*/