#pragma once
#include "Arduino.h"


typedef enum {
	NO_TEST=0,
	STARTED=1,
	SUCCESS=2,
	FAILED=3,
	ERROR=4
} TestStatus;

class AutoTests
{
public:
	AutoTests();
	~AutoTests();

	bool loop();
private:
	int currentTestNumber = 0;
	TestStatus status = TestStatus::NO_TEST;
	bool PrepareTest();
	bool RunTest();
	void TestMessage(String message);
	String currentName = "NoTest";
	int step = 0;
	unsigned long testStartTS = 0;

	bool RunTestCaseNormalStart();
	

};

