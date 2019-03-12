#pragma once
#include <Arduino.h>

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


class Loger
{
public:
	static void SimpleLog(String message);
	static void Log(DebugLevel level, String message);
	static void Error(String message) {
		Log(D_ERROR, message);
	}
	static void Fatal(String message) {
		Log(D_FATAL, message);
	}
	static void Info(String message) {
		Log(D_INFO, message);
	}
	static void Warn(String message) {
		Log(D_WARN, message);
	}
	static void Debug(String message) {
		Log(D_DEBUG, message);
	}
};

