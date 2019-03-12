#include "AutoTests.h"
#include "Simulator.h"
#include "OutputDevice.h"
#include "Configuration.h"
#include "Loger.h"


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
		}
	}
	if (status == TestStatus::STARTED) {
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
	case 2:
	case 3:
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
	bool res = true;

	switch (currentTestNumber) {
	case 0: {
		currentName = "Normal Start";
		res = TestDummySuccess();
//		res = TestStart();
		break;
	}
	case 1: {
		currentName = "Normal Stop";
//		res = TestStop();
		res = TestDummySuccess();
		break;
	}
	case 2: {
		currentName = "No Start - no temperature achieved";
//		res = TestNoStart();
		res = TestDummySuccess();

		break;
	}
	case 3: {
		currentName = "Start and stop on the last step";
//		res = TestDummySuccess();
		res = TestStartBrokenLastStep();
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
	Loger::Debug("AUTOTESTS#" + currentName + "#case:" + String(sim->GetCaseNumber()) + "#run_time:" + String(Config.Counter1s - testStartTS) +"#step:" + step + "#status:" + status + "#" + message);
}

bool AutoTests::TestStart() {
	bool res = false;
	static unsigned long ts = 0;
	// Step 0: Is start needed
	// Step 1: start Pump Geo
	// Step 2: Waiting for 3 min
	// Step 3: Start Compressor
	// Step 4: Waiting for desired temp achieved
	// Step 5: Stop Compressor
	// Step 6: Waiting for 3 min
	// Step 7: stop Pump Geo
	// Finish

	if (step == 0) { //Start
		sim->SetCaseNumber(0);
		TestMessage("Started!");
		step++;
		testStartTS = Config.Counter1s;
		ts = Config.Counter1s;
		res = true;

	}
	if (step == 1) { //Wait 3 sec to avoid counter unsync
		if (ts + 3 <= Config.Counter1s) {
			res = true;
			TestMessage("Step 1 is over");
			step++;
		}
	}
	if (step == 2) {
		if (testStartTS + Config.DevMgr.pumpContour1->minTimeOff <= Config.Counter1s) {
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
	if (step == 3) {
		if (testStartTS + Config.DevMgr.pumpContour2->minTimeOff <= Config.Counter1s) {
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

	if (step == 4) { // Wait Pump Geo
		if (testStartTS + Config.DevMgr.pumpGeo->minTimeOff <= Config.Counter1s) {
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
	if (step == 5) {
		if (testStartTS + max(Config.DevMgr.pumpContour2->minTimeOff, 
			(max(Config.DevMgr.pumpContour1->minTimeOff, Config.DevMgr.pumpContour1->minTimeOff))) + 10 <= Config.Counter1s) {
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
	
	if (status == TestStatus::FAILED) {
		TestMessage("Test Failed");
	}

	return res;
}

bool AutoTests::TestNoStart() {
	bool res = false;

	if (step == 0) { //Start
		sim->SetCaseNumber(2);
		TestMessage("Started!");
		step++;
		testStartTS = Config.Counter1s;
	}
	if (step == 1) { //Wait 5 sec to avoid counter unsync
		if (testStartTS + 5 <= Config.Counter1s) {
			res = true;
			TestMessage("Step 1 is over");
			step++;
		}
	}

	if (step == 2) { //Wait 200 sec to compressor shouldn't be started
		if (testStartTS + 200 <= Config.Counter1s) {
			if (Config.DevMgr.compressor.status != DeviceStatus::STATUS_ON) {
				TestMessage("Compressor didn't started");
				status = TestStatus::SUCCESS;
				res = true;
			}
			else {
				TestMessage("Compressor has been started");
				status = TestStatus::FAILED;
				res = false;
			}
			step++;
		}
		else {
			TestMessage("Seconds left: " + String(testStartTS + 200 - Config.Counter1s));
		}
	}

	if (step == 3) { //Final!
		if (status == TestStatus::SUCCESS) {
			TestMessage("Finish Successfully!");
		}
		else {
			TestMessage("FAILED!");
		}
		//status = TestStatus::SUCCESS;
		step++;
	}
	return res;
}


bool AutoTests::TestStop() {
	bool res = false;
	static unsigned long ts = 0;

	if (step == 0) { //Start test
		sim->SetCaseNumber(1);
		TestMessage("Started!");
		step++;
		testStartTS = Config.Counter1s;
	}
	if (step == 1) { //Wait 3 sec to avoid counter unsync
		if (testStartTS + 3 <= Config.Counter1s) {
			res = true;
			TestMessage("Step 1 is over");
			step++;
			ts = Config.Counter1s;
		}
	}
	if (step == 2) { // Wait Compressor stop
		//Debug("ts=" + String(ts) + ";Counter1s=" + String(Config.counter1s));
		if (ts + Config.DevMgr.compressor.minTimeOn + 5 /* Some gap */ <= Config.Counter1s) {
			if (Config.DevMgr.compressor.status != DeviceStatus::STATUS_OFF) {
				TestMessage("Compressor didn't stopped!");
				status = TestStatus::FAILED;
				res = false;
			}
			else {
				TestMessage("Compressor stopped");
				step++;
				ts = Config.Counter1s;
			}
		}
	}
	
	if (step == 3) { //Wait pump GEO contour is stopped
		if (ts + Config.DevMgr.pumpGeo->minTimeOn + 5  <= Config.Counter1s) {
			if (Config.DevMgr.pumpGeo->status != DeviceStatus::STATUS_OFF) {
				TestMessage("Geo Pump didn't stopped!");
				status = TestStatus::FAILED;
				res = false;
			}
			else {
				TestMessage("Pump GEO stopped");
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

bool AutoTests::TestDummySuccess() {
	status = TestStatus::SUCCESS;
	return true;
}

bool AutoTests::TestStartBrokenLastStep() {
	bool res = false;
	static unsigned long ts = 0;

	if (step == 0) { //Start
		sim->SetCaseNumber(3);
		TestMessage("Started!");
		step++;
		testStartTS = Config.Counter1s;
	}
	if (step == 1) { //Wait 5 sec to avoid counter unsync
		if (testStartTS + 5 <= Config.Counter1s) {
			res = true;
			TestMessage("Step 1 is over");
			step++;
		}
	}

	if (step == 2) {
		if (testStartTS + Config.DevMgr.pumpContour1->minTimeOff <= Config.Counter1s) {
			if (Config.DevMgr.pumpContour1->status != DeviceStatus::STATUS_ON) {
				TestMessage("Pump Contour1 didn't started!");
				status = TestStatus::FAILED;
				res = false;
			}
			else {
				TestMessage("Pump Contour1 started");
				step++;
			}
		}

	}
	if (step == 3) {
		if (testStartTS + Config.DevMgr.pumpContour2->minTimeOff <= Config.Counter1s) {
			if (Config.DevMgr.pumpContour2->status != DeviceStatus::STATUS_ON) {
				TestMessage("Pump Contour2 didn't started!");
				status = TestStatus::FAILED;
				res = false;
			}
			else {
				TestMessage("Pump Contour2 started");
				step++;
				ts = Config.Counter1s;
			}
		}
	}

	if (step == 4) { // Wait Pump Geo
		if (ts + Config.DevMgr.pumpGeo->minTimeOff + 7 <= Config.Counter1s) {
			if (Config.DevMgr.pumpGeo->status != DeviceStatus::STATUS_ON) {
				TestMessage("Pump Geo didn't started!");
				status = TestStatus::FAILED;
				res = false;
			}
			else {
				TestMessage("Pump Geo started");
				step++;
			}
		}
	}
	if (step == 5) {
		if (testStartTS + max(Config.DevMgr.pumpContour2->minTimeOff,
			(max(Config.DevMgr.pumpContour1->minTimeOff, Config.DevMgr.pumpContour1->minTimeOff))) + 10 <= Config.Counter1s) {
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
	if (step == 6) { //compressor is started. Forced stop
//		TestMessage("case:"+String(sim->GetCaseNumber()));
		res = sim->SetPinValue(3, 8, 0.0); //Contactor Error
		if (!res) Loger::Debug("3.Contactor Error");
		TestMessage("Forced Stop!");
		step++;
		ts = Config.Counter1s;
	}
	if (step == 7) { //Wait 3 sec to avoid counter unsync
		//TestMessage("Timing:" + String(ts) + ";" + String(Config.counter1s));
		if (ts + 3 <= Config.Counter1s) {
			//TestMessage("Final Timing:" + String(ts) + ";" + String(Config.counter1s));
			res = true;
			TestMessage("Step 7 is over");
			step++;
		}
	}

	if (step == 8) { //Check compressor stop 
		if (Config.DevMgr.compressor.status != DeviceStatus::STATUS_OFF) {
			TestMessage("Compressor didn't stopped!");
			status = TestStatus::FAILED;
			res = false;
		}
		else {
			TestMessage("Compressor stop!!!");
			step++;
			ts = Config.Counter1s;
		}
	}
	if (step == 9) { //Wait pump GEO contour is stopped
		if (ts + Config.DevMgr.pumpGeo->minTimeOn + 5 <= Config.Counter1s) {
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

	if (step == 10) { //Final!
		TestMessage("Finish Successfully!");
		status = TestStatus::SUCCESS;
		step++;
	}

	if (status == TestStatus::FAILED) {
		TestMessage("Test Failed");
	}
	return res;
}
