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
	static void SimpleLog(const String &message);
	static void Log(DebugLevel level, const String &message);
	static void Error(const String &message) {
		Log(D_ERROR, message);
	}
	static void Fatal(const String &message) {
		Log(D_FATAL, message);
	}
	static void Info(const String &message) {
		Log(D_INFO, message);
	}
	static void Warn(const String &message) {
		Log(D_WARN, message);
	}
	static void Debug(const String &message) {
		Log(D_DEBUG, message);
	}
};

