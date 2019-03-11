#pragma once
#include "Arduino.h"

class MqttCredentials
{
public:
	MqttCredentials();
	~MqttCredentials();

	String ServerURL = "";
	int Port = 0;
	String Login = "";
	String Password = "";
	String Root = "";
};

