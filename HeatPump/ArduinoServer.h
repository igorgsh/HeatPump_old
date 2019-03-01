#pragma once
#include "Arduino.h"
#include "Ethernet.h"
#include "HttpRequest.h"




class ArduinoServer
{
public:
	void begin();
	bool loop();
	ArduinoServer();
	~ArduinoServer();
private:
	byte mac[6] = { 0x00, 0xAA, 0x22, 0x07, 0x69, 0x07 };
	byte serverPort = 80;
	EthernetServer* server;
	int ProcessRequest(Client& client);
	void PrintErrorPage(Client& client, String error, String reason);
	void ParseCommand(Client& client, HttpRequest request);
	//void PrintMainPage(Client& client, HttpRequest request);
	void PrintHtmPage(Client& client, HttpRequest request);
	void PrintAnyFile(Client& client, HttpRequest request);
	void PrintSchedulePage(Client& client, HttpRequest request);
	void HttpHeader(Client& client, String error);
	bool isTplMonitorActual = false;
};

