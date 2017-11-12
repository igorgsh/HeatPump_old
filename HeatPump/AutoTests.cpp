#include "AutoTests.h"
#include "Definitions.h"
#include "Simulator.h"
#include "OutputDevice.h"
#include "Configuration.h"


extern Simulator *sim;
extern Configuration Config;

AutoTests::AutoTests()
{
}


AutoTests::~AutoTests()
{
}

bool AutoTests::loop() {
	bool res = false;

	if (status == TestStatus::NO_TEST) {
		res = PrepareTest();
		if (res) {
			status = TestStatus::STARTED;
			res = RunTest();
		}
	}
	else if (status == TestStatus::STARTED) {
		res = RunTest();
	}
	else if (status==TestStatus::SUCCESS || status==TestStatus::FAILED) {
		currentTestNumber++;
		status = TestStatus::NO_TEST;
		res = true;
	} 
	return res;
}

bool AutoTests::PrepareTest() {

	bool res = false;

	switch (currentTestNumber) {
	case 0: {
	case 1:
		step = 0;
		res = true;

		break;
	}
	default: {
		status = TestStatus::ERROR;
		currentName = "NoTest";
		TestMessage("Case=" + String(currentTestNumber));
		res = false;
		break;
	}
	}

	return res;
}

bool AutoTests::RunTest() {
	bool res = false;

	switch (currentTestNumber) {
	case 0: {
		currentName = "Normal Start";
		res = TestStart();
		break;
	}
	case 1: {
		currentName = "Normal Stop";
		res = TestStop();
		break;
	}
	default: {
		status = TestStatus::ERROR;
		currentName = "NoTest";
		TestMessage("Case=" + String(currentTestNumber));
		res = false;
		break;
	}
	}

	return res;

}

void AutoTests::TestMessage(String message) {
	Debug("AUTOTESTS#" + currentName + "#" + String(Config.counter1s - testStartTS) +"#" + step + "#" + status + "#" + message);
}
/*
bool AutoTests::TestNormalStartStop() {

	bool res = false;
	if (step <= 6) {
		TestStart(0);
	}
	else if (step<7+5) {
		TestStop(7);
	}
	else {
		res = true;
	}

	return res;
}
*/
bool AutoTests::TestStart() {
	bool res = false;

	if (step == 0) { //Start
		sim->SetCaseNumber(0);
		TestMessage("Started!");
		step++;
		testStartTS = Config.counter1s;
	}
	if (step== 1) { //Wait 5 sec to avoid counter unsync
		if (testStartTS + 5 <= Config.counter1s) {
			res = true;
			TestMessage("Step 1 is over");
			step++;
		}
	}
	if (step == 2) { // Wait Pump Geo
		if (testStartTS + Config.DevMgr.pumpGeo->minTimeOff <= Config.counter1s) {
			if (Config.DevMgr.pumpGeo->status != DeviceStatus::STATUS_ON) {
				TestMessage("Pump Geo didn't started!");
				status = TestStatus::FAILED;
				res = false;
			}
			else {
				TestMessage("Pump Geo is started");
				step++;
			}
		}
	}
	if (step == 3) {
		if (testStartTS + Config.DevMgr.pumpContour1->minTimeOff <= Config.counter1s) {
			if (Config.DevMgr.pumpContour1->status != DeviceStatus::STATUS_ON) {
				TestMessage("Pump Contour1 didn't started!");
				status = TestStatus::FAILED;
				res = false;
			}
			else {
				TestMessage("Pump Contour1 is started");
				step++;
			}
		}

	}
	if (step == 4) {
		if (testStartTS + Config.DevMgr.pumpContour2->minTimeOff <= Config.counter1s) {
			if (Config.DevMgr.pumpContour2->status != DeviceStatus::STATUS_ON) {
				TestMessage("Pump Contour2 didn't started!");
				status = TestStatus::FAILED;
				res = false;
			}
			else {
				TestMessage("Pump Contour2 is started");
				step++;
			}
		}

	}
	if (step == 5) {
		if (testStartTS + max(Config.DevMgr.pumpContour2->minTimeOff, 
			(max(Config.DevMgr.pumpContour1->minTimeOff, Config.DevMgr.pumpContour1->minTimeOff))) + 10 <= Config.counter1s) {
			if (Config.DevMgr.compressor.status != DeviceStatus::STATUS_ON) {
				TestMessage("Pump Compressor didn't started!");
				status = TestStatus::FAILED;
				res = false;
			}
			else {
				TestMessage("Compressor is started");
				step++;
			}
		}

	}

	if (step == 6) { //Final!
		TestMessage("Finish Successfully!");
		status = TestStatus::SUCCESS;
		step++;
	}
	return res;
}


bool AutoTests::TestStop() {
	bool res = false;

	if (step == 0) { //Start test
		sim->SetCaseNumber(1);
		TestMessage("Started!");
		step++;
		testStartTS = Config.counter1s;
	}
	if (step == 1) { //Wait 5 sec to avoid counter unsync
		if (testStartTS + 5 <= Config.counter1s) {
			res = true;
			TestMessage("Step 1 is over");
			step++;
		}
	}
	if (step == 2) { // Wait Compressor stop
		if (testStartTS + Config.DevMgr.compressor.minTimeOn +5 /* Some gap */ <= Config.counter1s) {
			if (Config.DevMgr.compressor.status != DeviceStatus::STATUS_OFF) {
				TestMessage("Compressor didn't stopped!");
				status = TestStatus::FAILED;
				res = false;
			}
			else {
				TestMessage("Compressor is stopped");
				step++;
			}
		}
	}
	
	if (step == 3) { //Wait pump GEO contour is stopped
		if (testStartTS + Config.DevMgr.pumpGeo->minTimeOn + 10  <= Config.counter1s) {
			if (Config.DevMgr.pumpGeo->status != DeviceStatus::STATUS_OFF) {
				TestMessage("Geo Pump didn't stopped!");
				status = TestStatus::FAILED;
				res = false;
			}
			else {
				TestMessage("Pump GEO is stopped");
				step++;
			}
		}

	}
	if (step == 4) { //Final!
		TestMessage("Finish Successfully!");
		status = TestStatus::SUCCESS;
		step++;
	}
	return res;
}