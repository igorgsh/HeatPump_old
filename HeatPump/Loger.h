#pragma once
#include <Arduino.h>

typedef enum {
	OFF = 0,
	FATAL = 1,
	ERROR = 2,
	WARN = 3,
	INFO = 4,
	DEBUG = 5,
	ALL = 6
} DebugLevel;



//#include "definitions.h"


class Loger
{
public:
	static void Log(DebugLevel level, String message);
	static void Error(String message) {
		Log(DebugLevel::ERROR, message);
	}
	static void Fatal(String message) {
		Log(DebugLevel::FATAL, message);
	}
	static void Info(String message) {
		Log(DebugLevel::INFO, message);
	}
	static void Warn(String message) {
		Log(DebugLevel::WARN, message);
	}
	static void Debug(String message) {
		Log(DebugLevel::DEBUG, message);
	}
};

