#include "ArduinoServer.h"
#include "Definitions.h"
#include "Configuration.h"
#include "HttpRequest.h"
#include "SD.h"
#include "SPI.h"

extern Configuration Config;


ArduinoServer::ArduinoServer()
{
	server = new EthernetServer(serverPort);
}


ArduinoServer::~ArduinoServer()
{
	//delete server;
}

void ArduinoServer::begin() {

	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP");
		// no point in carrying on, so do nothing forevermore:
		// try to congifure using IP address instead of DHCP:
		IPAddress ip(192, 168, 0, 101);
		Ethernet.begin(mac, ip);
	}
	server->begin();
	Debug2("Server is at: ",Ethernet.localIP());

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
		Debug("Client disconnected");
		delay(1);
	}
	return result;
}


int ArduinoServer::ProcessRequest(Client& client) {
	String line;

	// Read type of request
	line = client.readStringUntil('/');
	Debug2("TYPE=",line);
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
	Debug2("URL=",line);
	request.URL = line;

	// skip HTTP/1.1
	line = client.readStringUntil(0x0a);
	Debug2("Line=", line);
	//Serial.println(line);

	//Read parameters of header
	while (client.available()) {
		line = client.readStringUntil(0x0a);
		Debug2("Line=", line);

		if (line.length() == 1 && line[0] == 0x0D) {// Empty line. The body will be next
			request.body = client.readString();
		}
		else if (line.startsWith("Host: ")) {
			request.host = line.substring(6);
		}
	}
	request.ParseParameters();
	Debug2("Result:", (request.type == GET ? "Type: GET" : "Type: POST"));
	Debug2("URL:",request.URL);
	Debug2("Host:",request.host);
	Debug2("Body:",request.body);
	Debug2("NumberOfParm:", request.NumberParms);

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

	Debug("Start ParseCommand");
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
	Debug("End ParseCommand");
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

	Debug("Start PrintAnyFile");

	File file = SD.open(request.URL, FILE_READ);
	if (file) {
		HttpHeader(client, "200 Ok");
		while (file.available()) {
			uint8_t buf[100];
			file.readBytes(buf, 100);
			client.write(buf, 100);
		}
		file.close();
	}
	else
	{
		Debug("File not found");
		PrintErrorPage(client, "404 Not Found", "File:" + request.URL);
	}
	Debug("End PrintAnyFile");

}


String GetSensorParams(Sensor* sensor, String tpl) {
	String res = "";
	if (tpl.equals("%ArraySensorLabel%")) {
		res = sensor->getLabel();
	} else if (tpl.equals("%ArrayType%")) {
		res = sensor->getType();
	}
	else if (tpl.equals("%ArrayCode%")) {
		res = sensor->getPin();
	}
	else if (tpl.equals("%ArrayValue%")) {
		res = sensor->getValue();
	}
	else if (tpl.equals("%ArrayError%")) {
		res = sensor->getActionStatus();
	}
	else if (tpl.equals("%ArrayCritical%")) {
		res = sensor->isCritical();
	}
	else if (tpl.equals("%ArrayCriticalCounter%")) {
		res = sensor->getCriticalThreshold();
	}
	/*
	else if(tpl.equals("%ArrayAlarmLow%")) {
		res = sensor->getAlarmLow();
	}
	else if (tpl.equals("%ArrayAlarmHigh%")) {
		res += sensor->getAlarmHigh();
	}
	*/
	else if (tpl.equals("%ArrayStartLow%")) {
		res = sensor->getActionLow();
	}
	else if (tpl.equals("%ArrayStartHigh%")) {
		res = sensor->getActionHigh();
	}
	return res;
}

String GetTemplate(String tpl) {
	String res = "";
	if (tpl.startsWith("%Array")) {
		res = "[";
		for (int i = 0; i < Config.DevManager.getNumberTemp(); i++) {
			res += "'";
			res += GetSensorParams(&(Config.DevManager.tempSensors[i]), tpl);
			res += "',";
		}
		/*
		for (int i = 0; i < Config.DevManager.getNumberCont(); i++) {
			res += "'";
			res += GetSensorParams(&(Config.DevManager.contacts[i]), tpl);
			res += "',";
		}
		*/
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
	if (ind < Config.DevManager.getNumberTemp()) {
		s = &(Config.DevManager.tempSensors[ind]);
	}
	else {
		//s = &(Config.contacts[ind - Config.getNumberTemp()]);
	}
	if (varName.equals("arrayLabel")) {
		s->setLabel(decodeHex(val));
		//s->setLabel(val);
	}
	else if (varName.equals("arrayCritCnt")) {
		s->setCriticalThreshold(val.toInt());
	}
	/*
	else if (varName.equals("arrayAlarmLow")) {
		s->setAlarmLow(val.toFloat());
	}
	else if (varName.equals("arrayAlarmHigh")) {
		s->setAlarmHigh(val.toFloat());
	}
	*/
	else if (varName.equals("arrayStartLow")) {
		s->setActionLow(val.toFloat());
	}
	else if (varName.equals("arrayStartHigh")) {
		s->setActionHigh(val.toFloat());
	}
}

void ArduinoServer::PrintHtmPage(Client& client, HttpRequest request) {

	Debug("Start PrintMonitorPage");
	
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
				/*
				ind = request.getIndexOfParmKey("desTemp");
				if (ind != -1) {
					Config.cThermo.setDesiredTemp(request.getParmValue(ind).toFloat());
				}
				*/
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

				if (s.startsWith("%TEMPLATE_START%")) {
					isTpl = true;
				}
				else if (s.startsWith("%TEMPLATE_END%")) {
					isTpl = false;
					isStart = false;
				}
				else if (isTpl) {
					String tpl = "";
					s.replace("%ArraySensorLabel%", GetTemplate("%ArraySensorLabel%"));
					s.replace("%ArrayType%", GetTemplate("%ArrayType%"));
					s.replace("%ArrayCode%", GetTemplate("%ArrayCode%"));
					s.replace("%ArrayValue%", GetTemplate("%ArrayValue%"));
					s.replace("%ArrayError%", GetTemplate("%ArrayError%"));
					s.replace("%ArrayCritical%", GetTemplate("%ArrayCritical%"));
					s.replace("%ArrayCriticalCounter%", GetTemplate("%ArrayCriticalCounter%"));
					s.replace("%ArrayAlarmLow%", GetTemplate("%ArrayAlarmLow%"));
					s.replace("%ArrayAlarmHigh%", GetTemplate("%ArrayAlarmHigh%"));
					s.replace("%ArrayStartLow%", GetTemplate("%ArrayStartLow%"));
					s.replace("%ArrayStartHigh%", GetTemplate("%ArrayStartHigh%"));

/*
					s.replace("%DesiredTemperature%", String(Config.cThermo.getDesiredTemp()));
					s.replace("%CurrentTemperature%", String(Config.cThermo.getTemp()));
*/
					s.replace("%CurrentTime%", "Date-Time here");



					client.println(s);
					Debug(s);
				}
				else { // ordinary line - nothing special
					client.println(s);
					//Debug(s);
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
		Debug("File not found");
		PrintErrorPage(client, "404 Not Found", "File:" + request.URL);
	}
	Debug("End PrintMainPage");

}

void ArduinoServer::PrintSchedulePage(Client& client, HttpRequest request) {

}
