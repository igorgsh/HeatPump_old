#pragma once
#include "Arduino.h"
#include <IPAddress.h>

class MqttCredentials
{
public:
	IPAddress ServerIP;
	int Port = 0;
	String Login = "";
	String Password = "";
	String Root = "";
};

