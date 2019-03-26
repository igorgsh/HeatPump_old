#include "ArduinoServer.h"
#include "Configuration.h"
#include "HttpRequest.h"
#include "SD.h"
#include "SPI.h"
#include "Loger.h"
#include "TempSensor.h"

extern Configuration Config;

constexpr auto SDCARD_SS = 4;


ArduinoServer::ArduinoServer()
{
	server = new EthernetServer(serverPort);
	bool res = SD.begin(SDCARD_SS);
	Loger::SimpleLog("SDCARD.begin=" + String(res));
}


ArduinoServer::~ArduinoServer()
{
	//delete server;
}

bool ArduinoServer::setup() {
	server->begin();
	return true;
}

bool ArduinoServer::loop() {
	bool result = true;
	EthernetClient client = server->available();
	while (client.available()) {
		if (ProcessRequest(client) != 0) {
			 //Error. Bad Request
			PrintErrorPage(client, "418 I'm a teapot", "Something wrong");
		}
		client.stop();
		//Debug("Client disconnected");
		delay(1);
	}
	return result;
}


int ArduinoServer::ProcessRequest(Client& client) {
	String line;

	// Read type of request
	line = client.readStringUntil('/');
	//Debug2("TYPE=",line);
	HttpRequest request;

	if (line.startsWith("GET")) {
		request.type = GET;
	}
	else if (line.startsWith("POST")) {
		request.type = POST;
	}
	else {
		return -1; //Unsupported type
	}
	// Read URL
	line = client.readStringUntil(' ');
	//Debug2("URL=",line);
	request.URL = line;

	// skip HTTP/1.1
	line = client.readStringUntil(0x0a);
	//Debug2("Line=", line);

	//Read parameters of header
	while (client.available()) {
		line = client.readStringUntil(0x0a);
		//Debug2("Line=", line);

		if (line.length() == 1 && line[0] == 0x0D) {// Empty line. The body will be next
			request.body = client.readString();
		}
		else if (line.startsWith("Host: ")) {
			request.host = line.substring(6);
		}
	}
	request.ParseParameters();
	//Debug2("Result:", (request.type == GET ? "Type: GET" : "Type: POST"));
	//Debug2("URL:",request.URL);
	//Debug2("Host:",request.host);
	//Debug2("Body:",request.body);
	//Debug2("NumberOfParm:", request.NumberParms);

	ParseCommand(client, request);

	return 0;
}

void ArduinoServer::PrintErrorPage(Client& client, String error, String reason) {
	
	HttpHeader(client, error);
	client.println(error);
	client.println(reason);
}

void ArduinoServer::HttpHeader(Client& client, String error) {

	client.println("HTTP/1.1 " + error);
	client.println("Content-Type: text/html; charset=utf-8");
	//client.println("Cache-Control: no-cache, must-revalidate");
	//client.println("Pragma: no-cache");
	//client.println("Expires: Sat, 26 Jul 1997 05:00:00 GMT");
	client.println("Connection: close");  // the connection will be closed after completion of the response
	client.println();
}

//#define NUMBER_OF_PARTS	5


void ArduinoServer::ParseCommand(Client& client, HttpRequest request) {

	//Debug("Start ParseCommand");

	if (request.URL.length() == 0) { //open main.htm when no page defined
		request.URL = "main.htm";
	}

	if (request.URL.endsWith(".htm")) {
		PrintHtmPage(client, request);
	}
	else {
		PrintAnyFile(client, request);
	}
	/*
	if (request.URL.startsWith("main.htm")) {
		PrintMainPage(client, request);
	}
	else if (request.URL.startsWith("monitor.htm")) {
		PrintMonitorPage(client, request);
	}
	else if (request.URL.startsWith("schedule.htm")) {
		PrintSchedulePage(client, request);
	}
	else {
		PrintErrorPage(client, "404 Page not found", "This page is unavailable");
	}
	*/
	//Debug("End ParseCommand");
}
/*
void ArduinoServer::PrintMainPage(Client& client, HttpRequest request) {

	Debug("Start PrintMainPage");
	if (request.NumberParms != 0) {
		int ind = request.getIndexOfParmKey("desTemp");
		if (ind!=-1) {
			Config.cThermo.setDesiredTemp(request.getParmValue(ind).toFloat());
		}
	}
	Debug2("Desired Temp=", Config.cThermo.getDesiredTemp());

	File pageTpl = SD.open("main.htm", FILE_READ);
	if (pageTpl) {
		HttpHeader(client, "200 Ok");
		while (pageTpl.available()) {
			String s = pageTpl.readStringUntil(0x0A);
			s.replace("%DesiredTemperature%", String(Config.cThermo.getDesiredTemp()));
			s.replace("%CurrentTemperature%", String(Config.cThermo.getTemp()));
			s.replace("%CurrentTime%", "тут будет дата-время");
			client.println(s);
		}
		pageTpl.close();
	}
	else
	{
		Debug("File not found");
		PrintErrorPage(client, "404 Not Found", "Проблема с чтением темплейта");
	}
	Debug("End PrintMainPage");

}
*/

void ArduinoServer::PrintAnyFile(Client& client, HttpRequest request) {

	//Debug("Start PrintAnyFile");
	
	File file = SD.open(request.URL, FILE_READ);
	
	if (file) {
		HttpHeader(client, "200 Ok");
		while (file.available()) {
			uint8_t buf[100];
			size_t sz = file.readBytes(buf, 100);
			client.write(buf, sz);
		}
		file.close();
	}
	else
	{
		Loger::Debug("File not found:" + request.URL);
		PrintErrorPage(client, "404 Not Found", "File:" + request.URL);
	}
	Loger::Debug("End PrintAnyFile");

}


String GetSensorParams(TempSensor* sensor, String tpl) {
	String res = "";
	if (tpl.equals("%SensorsLabel%")) {
		res = sensor->GetLabel();
	} else if (tpl.equals("%SensorsType%")) {
		res = String(sensor->GetType());
	}
	else if (tpl.equals("%SensorsCode%")) {
		res = String(sensor->GetPin());
	}
	else if (tpl.equals("%SensorsValue%")) {
		res = String(sensor->GetValue());
	}
	else if (tpl.equals("%SensorsError%")) {
		res = String(sensor->GetActionStatus()==ActionStatus::ACTION_ALARM);
	}
	else if (tpl.equals("%SensorsLowerRange%")) {
		res = String(sensor->GetLowerRange());
	}
	else if (tpl.equals("%SensorsUpperRange%")) {
		res = String(sensor->GetUpperRange());
	}
	else if (tpl.equals("%SensorsMAC%")) {
		//Loger::Debug("Sensor:" + sensor->GetLabel());
		//Loger::Debug("DEvAddr: " + String(sensor->GetMac()[0],HEX) + String(sensor->GetMac()[1], HEX));
		res = Config.PrintMac(sensor->GetMac());
		//res = Config.PrintMac(NULL);
	}

	return res;
}

String GetPumpParams(Pump* pump, String tpl) {
	String res = "";
	if (tpl.equals("%PumpsLabel%")) {
		res = pump->GetLabel();
	}
	else if (tpl.equals("%PumpsStatus%")) {
		res = String(pump->status);
	}
	return res;
}


String GetTemplate(String tpl) {
	String res = "";
	//Loger::Debug("TPL:" + tpl);
	if (tpl.startsWith("%Sensors")) {
		res = "[";
		for (int i = 0; i < Config.DevMgr.getNumberTemp(); i++) {
			TempSensor* ts = &(Config.DevMgr.tempSensors[i]);
			res += "'";
			res += GetSensorParams(ts, tpl);
			res += "',";
		}
		/*
		for (int i = 0; i < Config.DevMgr.getNumberCont(); i++) {
			res += "'";
			res += GetSensorParams(&(Config.DevMgr.contacts[i]), tpl);
			res += "',";
		}
		*/
		res[res.length() - 1] = ']';
	}
	else if (tpl.startsWith("%Pumps")) {
		res = "[";
		for (int i = 0; i < Config.DevMgr.getNumberPump(); i++) {
			res += "'";
			res += GetPumpParams(&(Config.DevMgr.pumps[i]), tpl);
			res += "',";
		}
		res[res.length() - 1] = ']';
	}
	return res;
}

byte hex2Byte(char c) {
	byte res = 0;

	if (c>='0' && c<='9') {
		res = c - '0';
	}
	else if (c >= 'A' && c <= 'F') {
		res = 10 + (c - 'A');
	}
	else if (c >= 'a' && c <= 'f') {
		res = 10 + (c - 'a');
	}
	//Debug2("C=", c);
	//Debug2("RES=", res);
	return res;
}

String decodeHex(String s) {
	String res = "";

	for (unsigned int i = 0; i < s.length(); i++) {
		if (s[i] == '%') {

			res += char((hex2Byte(s[i + 1]) <<4) + hex2Byte(s[i + 2]));
			i += 2;
		}
		else {
			res += s[i];
		}
	}
	return res;
}

void setArrayVar(String varName, int ind, String val) {
	Sensor *s;
	if (ind < Config.DevMgr.getNumberTemp()) {
		s = &(Config.DevMgr.tempSensors[ind]);
	}
	else {
		//s = &(Config.contacts[ind - Config.getNumberTemp()]);
	}
	if (varName.equals("arrayLabel")) {
		s->SetLabel(decodeHex(val));
		//s->setLabel(val);
	}
}

void ArduinoServer::PrintHtmPage(Client& client, HttpRequest request) {

	//Debug("Start PrintMonitorPage");
	
	if (request.NumberParms != 0) { // Some POST variables submitted
		//int ind;

		for (int i = 0; i < request.NumberParms; i++) {
			if (request.ParamKeys[i].startsWith("array")) {
				int n = request.ParamKeys[i].indexOf("_");
				if (n > 0) {
					int numb = request.ParamKeys[i].substring(n + 1).toInt();
					String var = request.ParamKeys[i].substring(0, n);
					setArrayVar(var, numb, request.ParamValues[i]);
				}
			}
			else {
				
				int ind = request.getIndexOfParmKey("desTemp");
				if (ind != -1) {
					Config.SetDesiredTemp(request.getParmValue(ind).toFloat());
				}
				
			}
		}

	}
	

	File pageTpl = SD.open(request.URL, FILE_READ);
	if (pageTpl) {
		HttpHeader(client, "200 Ok");
		bool isTpl = false;
		bool isStart = true;
		while (pageTpl.available()) {
			if (isStart) {
				String s = pageTpl.readStringUntil(0x0A);
				//Loger::Debug(s);
				if (s.startsWith("%TEMPLATE_START%")) {
					isTpl = true;
				}
				else if (s.startsWith("%TEMPLATE_END%")) {
					isTpl = false;
					isStart = false;
				}
				else if (isTpl && isStart) {
					//String tpl = "";
					s.replace("%SensorsLabel%", GetTemplate("%SensorsLabel%"));
					s.replace("%SensorsType%", GetTemplate("%SensorsType%"));
					s.replace("%SensorsCode%", GetTemplate("%SensorsCode%"));
					s.replace("%SensorsValue%", GetTemplate("%SensorsValue%"));
					s.replace("%SensorsError%", GetTemplate("%SensorsError%"));
					s.replace("%SensorsLowerRange%", GetTemplate("%SensorsLowerRange%"));
					s.replace("%SensorsUpperRange%", GetTemplate("%SensorsUpperRange%"));
					s.replace("%SensorsMAC%", GetTemplate("%SensorsMAC%"));

					s.replace("%DesiredTemperature%", String(Config.GetDesiredTemp()));
					s.replace("%CurrentTemperature%", String(Config.GetCurrentTemp()));

					s.replace("%CurrentTime%", "'Date-Time here'");

					s.replace("%CompressorStatus%", String(Config.DevMgr.compressor.status));

					s.replace("%PumpsLabel%", GetTemplate("%PumpsLabel%"));
					s.replace("%PumpsStatus%",GetTemplate("%PumpsStatus%"));

					client.println(s);
					//Loger::Debug(s);
				}
				else { // ordinary line - nothing special
					client.println(s);
				}
			}
			else {
#define BUF_SIZE	100
				uint8_t buffer[BUF_SIZE];
				int sz = pageTpl.readBytes(buffer, BUF_SIZE);
				client.write(buffer, sz);
			}
		}
		pageTpl.close();
	}
	else
	{
		Loger::Debug("File not found:" + request.URL);
		PrintErrorPage(client, "404 Not Found!", "File:" + request.URL);
	}
	//Debug("End PrintMainPage");

}

void ArduinoServer::PrintSchedulePage(Client& client, HttpRequest request) {

}
