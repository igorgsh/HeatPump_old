#include "Loger.h"
#include "mqtt.h"
#include "Configuration.h"

extern Configuration Config;

void Loger::Log(DebugLevel level, const String &message) {
	if (level <= dLevel) {
		Serial.println(message);
		if (Config.IsMqtt()) {
			Config.GetMqttClient()->PublishLog(level, message);
		}
	}
}

void Loger::SimpleLog(const String &message) {
	Serial.println(message);
}