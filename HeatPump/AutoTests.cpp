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
		step = 0;
		//sim->SetValueGenNumber(currentTestNumber);
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
		currentName = "Normal Start and Stop";
		res = RunTestCaseNormalStart();
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
	Debug("AUTOTESTS#" + currentName + "#" + status + "#" + message);
}

bool AutoTests::RunTestCaseNormalStart() {
	bool res = false;

	if (step == 0) { //Start
		sim->SetCaseNumber(0);
		TestMessage("Started!");
		step = 1;
		testStartTS = Config.counter1s;
	}
	if (step==1) { //Wait 5 sec to avoid counter unsync
		if (testStartTS + 5 <= Config.counter1s) {
			res = true;
			TestMessage("Step 1 is over");
			step = 2;
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
				step = 3;
			}
		}
	}
	if (step == 3) { //Final!
		TestMessage("Finish Successfully!");
		status = TestStatus::SUCCESS;
	}
	return res;
}